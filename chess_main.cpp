#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <string>
#include <gl/gl.h>
#include <gl/glu.h>

#include "chess_texture_manage.h"
#include "chess_md2.h"
#include "chess_opengl.h"
#include "chess_game.h"
#include "chess_timer.h"

bool exiting = false;
long windowWidth = 1024;
long windowHeight = 768;
long windowBits = 24;
bool fullscreen = false;
int mouseX, mouseY;
HDC hDC;

ChessOGL* kRender = NULL;
ChessHiResTimer* kTimer = NULL;
ChessGame* kGame = NULL;

void setupPixelFormat(HDC hDC)
{
  int pixelFormat;

  PIXELFORMATDESCRIPTOR pfd =
  {	
    sizeof(PIXELFORMATDESCRIPTOR),	// size
    1,							// version
    PFD_SUPPORT_OPENGL |		// OpenGL window
    PFD_DRAW_TO_WINDOW |		// render to window
    PFD_DOUBLEBUFFER,			// support double-buffering
    PFD_TYPE_RGBA,				// color type
    32,							// prefered color depth
    0, 0, 0, 0, 0, 0,			// color bits (ignored)
    0,							// no alpha buffer
    0,							// alpha bits (ignored)
    0,							// no accumulation buffer
    0, 0, 0, 0,					// accum bits (ignored)
    16,							// depth buffer
    0,							// no stencil buffer
    0,							// no auxiliary buffers
    PFD_MAIN_PLANE,				// main layer
    0,							// reserved
    0, 0, 0,					// no layer, visible, damage masks
  };

  pixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, pixelFormat, &pfd);
}

LRESULT CALLBACK mainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;
  static HGLRC hRC;
  int height, width;
  int xPos, yPos;
  double x, y, z;
  char str[40] = {'\0'};

  // dispatch messages
  switch (uMsg) {
    case WM_CREATE: {    // window creation
      hDC = GetDC(hWnd);
      setupPixelFormat(hDC);
      hRC = wglCreateContext(hDC);
      wglMakeCurrent(hDC, hRC);
      break;
    }

    case WM_DESTROY:    // window destroy
    case WM_QUIT:
    case WM_CLOSE: {    // window is closing
      // deselect rendering context and delete it
      wglMakeCurrent(hDC, NULL);
      wglDeleteContext(hRC);

      // send WM_QUIT to message queue
      PostQuitMessage(0);
      break;
    }

    case WM_SIZE: {
      width = LOWORD(lParam);
      height = HIWORD(lParam);  // retrieve width and height

      kRender->setupProjection(width, height);
      break;
    }

    case WM_ACTIVATEAPP:    // activate app
      break;

    case WM_PAINT: {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
      break;
    }

    case WM_LBUTTONDOWN: {
      xPos = LOWORD(lParam);
      yPos = HIWORD(lParam);
      kRender->get3DIntersection(xPos, yPos, x, y, z);
      kGame->onSelection((float)z, (float)x);
      break;
    }

    case WM_RBUTTONDOWN:
      break;

    case WM_MOUSEMOVE:
      break;

    case WM_LBUTTONUP:
      break;

    case WM_RBUTTONUP:
      break;

    case WM_KEYUP:
      break;

    case WM_KEYDOWN: {
      int fwKeys;
      LPARAM keyData;
      fwKeys = (int)wParam; // virtual-key code
      keyData = lParam;   // key data

      switch (fwKeys) {
        case VK_ESCAPE: {
          PostQuitMessage(0);
          break;
        }

        default:
          break;
      }
      break;
    }
                     
    default:
      break;
  }

  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
                   int nShowCmd)
{
  WNDCLASSEX windowClass;    // window class
  HWND hwnd;    // window handle
  MSG msg;    // msg
  DWORD dwExStyle;    // window extended style
  DWORD dwStyle;    // window style
  RECT windowRect;

  kRender = new ChessOGL;
  kTimer = new ChessHiResTimer;
  kGame = new ChessGame;

  windowRect.left = (long)0; // set left value to 0
  windowRect.right = (long)windowWidth; // set right value ro requested width
  windowRect.top = (long)0; // set top value to 0
  windowRect.bottom = (long)windowHeight; // set bottom value to requested height

  // fill out the window class structure
  windowClass.cbSize = sizeof (WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = mainWindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);

  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

  windowClass.hbrBackground = NULL;
  windowClass.lpszMenuName = NULL;
  windowClass.lpszClassName = "ChessClass";
  windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

  if (!RegisterClassEx(&windowClass))
    return 0;

  if (fullscreen) {
    DEVMODE dmScreenSettings;

    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = windowWidth;
    dmScreenSettings.dmPelsWidth = windowHeight;
    dmScreenSettings.dmBitsPerPel = windowBits;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      MessageBox(NULL, "Display mode failed", NULL, MB_OK);
      fullscreen = false;
    }
  }

  if (fullscreen) {
    dwExStyle = WS_EX_APPWINDOW;
    dwStyle = WS_POPUP;
    ShowCursor(false);
  } else {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
  }

  AdjustWindowRectEx(&windowRect, dwStyle, false, dwExStyle);

  hwnd = CreateWindowEx(NULL,
    "ChessClass",
    "Chess3D",
    dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    0, 0,
    windowRect.right - windowRect.left,
    windowRect.bottom - windowRect.top,
    NULL,
    NULL,
    hInstance,
    NULL);

  hDC = GetDC(hwnd);

  if (!hwnd)
    return 0;

  ShowWindow(hwnd, SW_SHOW);
  UpdateWindow(hwnd);

  if (kRender->init()) {
    MessageBox(NULL, "ChessOGL::init() error!", "ChessOGL class failed to initialize",
      MB_OK);
    return -1;
  }

  kTimer->init();
  kGame->initialize();
  kRender->attachToGame(kGame);

  while (!exiting) {
    kRender->prepare(kTimer->getElapsedSeconds(1));
    kRender->render();
    SwapBuffers(hDC);

    while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
      if (!GetMessage(&msg, NULL, 0, 0)) {
        exiting = true;
        break;
      }

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  delete kGame;
  delete kTimer;
  delete kRender;

  if (fullscreen) {
    ChangeDisplaySettings(NULL, 0);
    ShowCursor(true);
  }
}