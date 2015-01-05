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

ChessOGL *kRender = NULL;
ChessHiResTimer *kHiResTimer = NULL;
ChessGame *kGame = NULL;

void setupPixelFormat(HDC hDC)
{
  int pixelFormat;

  PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),	// size
    1,							// version
    PFD_SUPPORT_OPENGL |		// OpenGL window
    PFD_DRAW_TO_WINDOW |		// render to window
    PFD_DOUBLEBUFFER,			// support double-buffering
    PFD_TYPE_RGBA,				// color type
    32,							// preferred color depth
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

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  static HDC hDC;
  static HGLRC hRC;
  int height, width;
  int xPos, yPos;
  double x, y, z;
  char str[40] = { '\0' };

  // dispatch messages
  switch (uMsg) {
    case WM_CREATE: {
      hDC = GetDC(hWnd);
      setupPixelFormat(hDC);
      //SetupPalette();
      hRC = wglCreateContext(hDC);
      wglMakeCurrent(hDC, hRC);
      break;
    }

    case WM_DESTROY:
    case WM_QUIT:
    case WM_CLOSE: {

      // deselect rendering context and delete it
      wglMakeCurrent(hDC, NULL);
      wglDeleteContext(hRC);

      // send WM_QUIT to message queue
      PostQuitMessage(0);
      break;
    }

    case WM_SIZE: {
      height = HIWORD(lParam);
      width = LOWORD(lParam);

      kRender->setupProjection(width, height);
      break;
    }

    case WM_ACTIVATEAPP:
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
      fwKeys = (int)wParam;    // virtual-key code 
      keyData = lParam;        // key data 

      switch (fwKeys) {
        case VK_ESCAPE:
          PostQuitMessage(0);
          break;

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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  WNDCLASSEX windowClass;  // window class
  HWND hwnd;			   // window handle
  MSG msg;				   // message
  DWORD dwExStyle;		   // Window Extended Style
  DWORD	dwStyle;		   // Window Style
  RECT windowRect;

  kRender = new ChessOGL;
  kHiResTimer = new ChessHiResTimer;
  kGame = new ChessGame;

  windowRect.left = (long)0;			   // Set Left Value To 0
  windowRect.right = (long)windowWidth;	   // Set Right Value To Requested Width
  windowRect.top = (long)0;				   // Set Top Value To 0
  windowRect.bottom = (long)windowHeight;  // Set Bottom Value To Requested Height

  // fill out the window class structure
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.style = CS_HREDRAW | CS_VREDRAW;
  windowClass.lpfnWndProc = MainWindowProc;
  windowClass.cbClsExtra = 0;
  windowClass.cbWndExtra = 0;
  windowClass.hInstance = hInstance;
  windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// default icon
  windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);	// default arrow
  windowClass.hbrBackground = NULL;								// don't need background
  windowClass.lpszMenuName = NULL;								// no menu
  windowClass.lpszClassName = "GLClass";
  windowClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);	// windows logo small icon

  // register the windows class
  if (!RegisterClassEx(&windowClass))
    return 0;

  if (fullscreen)								// fullscreen?
  {
    DEVMODE dmScreenSettings;					// device mode
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);
    dmScreenSettings.dmPelsWidth = windowWidth;			// screen width
    dmScreenSettings.dmPelsHeight = windowHeight;			// screen height
    dmScreenSettings.dmBitsPerPel = windowBits;				// bits per pixel
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
      // setting display mode failed, switch to windowed
      MessageBox(NULL, "Display mode failed", NULL, MB_OK);
      fullscreen = FALSE;
    }
  }

  if (fullscreen) {                 // Are We Still In fullscreen Mode?
    dwExStyle = WS_EX_APPWINDOW;      // Window Extended Style
    dwStyle = WS_POPUP;               // Windows Style
    ShowCursor(FALSE);              // Hide Mouse Pointer
  } else {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	// Window Extended Style
    dwStyle = WS_OVERLAPPEDWINDOW;					// Windows Style
  }

  // Adjust Window To True Requested Size
  AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

  // class registered, so now create our window
  hwnd = CreateWindowEx(NULL,			// extended style
                        "GLClass",							// class name
                        "Chess3D",                          // app name
                        dwStyle | WS_CLIPCHILDREN |
                        WS_CLIPSIBLINGS,
                        0, 0,								// x,y coordinate
                        windowRect.right - windowRect.left,
                        windowRect.bottom - windowRect.top, // width, height
                        NULL,								// handle to parent
                        NULL,								// handle to menu
                        hInstance,							// application instance
                        NULL);								// no extra params

  hDC = GetDC(hwnd);

  // check if window creation failed (hwnd would equal NULL)
  if (!hwnd)
    return 0;

  ShowWindow(hwnd, SW_SHOW);			// display the window
  UpdateWindow(hwnd);					// update the window

  if (!kRender->initialize()) {
    MessageBox(NULL, "ChessOGL::initialize() error!",
               "ChessOGL class failed to initialize!", MB_OK);
    return -1;
  }

  kHiResTimer->initialize();
  kGame->initialize();
  kRender->attachToGame(kGame);

  while (!exiting) {
    kRender->prepare(kHiResTimer->getElapsedSeconds(1));
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
  delete kHiResTimer;
  delete kRender;

  if (fullscreen) {
    ChangeDisplaySettings(NULL, 0);	// If So Switch Back To The Desktop
    ShowCursor(TRUE);				// Show Mouse Pointer
  }

  return (int)msg.wParam;
}