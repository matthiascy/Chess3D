#include "file_define.h"

#ifdef WIN32
#include "resource.h"
#endif

#define APP_NAME    "Multi Main"

processLife = 1;

void processLoop();

#if defined WIN32

HINSTANCE   kInst;    // current instance
HWND        kWnd;     // window handle

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hprevInstance,
		     LPSTR lpCmdLine, int nCmdShow)
{
  WNDCLASS wc;

  if (!hPrevInstance) {
    wc.style         = CS_HREDRAW;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbWndExtra    = 0;
    wc.cbClsExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(BLACK_BRUSH);
    wc.lpszMenuName  = (LPSTR)IDR_MENU1;
    wc.lpszClassName = APP_NAME;

    if (RegisterClass(&wc) == 0) return false;
  }

  kInst = hInstance;

  kWnd = CreateWindow(APP_NAME, APP_NAME,
		      WS_SYSMENU | WS_MINIMIZEBOX,
		      CW_USEDEFAULT, 0,
		      350, 44,
		      NULL, NULL,
		      hInstance, NULL);
  if (!kWnd) return false;

  ShowWindow(kWnd, nCmdShow);
  UpdateWindow(kWnd);

  processLoop();

  return WM_DESTROY;
}

LRESULT CALLBACK WndProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {
        case IDI_SHUTDOWN: {
	  if (MessageBox(kWnd, "Quit?", "Info", MB_YESNO) == IDNO)
	    break;
	  SendMessage(kWnd, WM_DESTROY, 0, 0);
	  processLife = 0;
	  break;
	}
	break;
      }
      break;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }

    default: {
      return DefWindsowProc(hWnd, uMsg, wParam, lParam);
    }
  }
  return 0;
}

void destroySignal(int sigNum)
{
  processLife = 0;
}

int main()
{
  struct sigaction act;

  act.sa_handler = SIG_IGN;
  act.sa_flags &= ~SA_RESETHAND;
  sigaction(SIGPIPE, &act, NULL);

  signal(SIGINT, destroySignal);
  signal(SIGKILL, destroySignal);
  signal(SIGQUIT, destroySignal);
  signal(SIGTERM, destroySignal);

  processLoop();

  return 1;
}

#endif

void processLoop()
{
#if defined (WIN32)
  MSG msg;
#endif
  
  while (processLife) {
#if defined (WIN32)
    
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
      if (!GetMessage(&msg, NULL, 0, 0))
	break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    
#endif
  }
}
