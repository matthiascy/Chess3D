#include "file_define.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"
#include "message.h"
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int processLife = true;

SOCKET serverSocket = INVALID_SOCKET;

ClientData_ptr clientList = NULL;

int totalClient = 0;

void processLoop();

#define APP_NAME "Chess3D_Server"

HINSTANCE hInst;    // current instance
HWND hWnd;          // window handle
MSG msg;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
  WNDCLASSEX wc;

  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = APP_NAME;
  wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

  if (!RegisterClassEx(&wc))
    return 0;

  hInst = hInstance;

  DWORD dwMenuHeight = GetSystemMetrics(SM_CYMENU);
  DWORD dwCaptionHeight = GetSystemMetrics(SM_CYCAPTION);
  DWORD dwFrameHeight = GetSystemMetrics(SM_CYSIZEFRAME);

  hWnd = CreateWindowEx(NULL,
                        APP_NAME,
                        APP_NAME,
                        WS_SYSMENU | WS_MINIMIZEBOX,
                        CW_USEDEFAULT, 0,
                        350, 
                        dwMenuHeight + dwCaptionHeight + dwFrameHeight,
                        NULL, NULL,
                        hInstance,
                        NULL);

  if (!hWnd) return false;

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  processLoop();

  return (WM_DESTROY);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg,
                         WPARAM wParam, LPARAM lParam);
{
  switch (uMsg) {
    case WM_COMMAND: {
      switch (LOWORD(wParam)) {
        case IDM_SHUTDOWN: {
          if (MessageBox(hWnd,
            "Terminate Server", "Confirm", MB_YESNO) == IDNO) break;
          SendMessage(hWnd, WM_DESTROY, 0, 0);
          processLife = false;
          break;
        }
      }
      break;
    }

    case WM_DESTROY: {
      PostQuitMessage(0);
      break;
    }

    default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  return 0;
}

voie initServerData() {}

void processLoop()
{
  WSADATA wsaData;
  WSAStartup(0x0101, &wsaData);

  serverSocket = initServerSock(SERVER_PORT, MAX_LISTEN);

  if (serverSocket == INVALID_SOCKET) return;

  initServerData();

  struct fd_set read_set;
  struct fd_set write_set;
  struct fd_set exc_set;
  struct timeval tv;
  SOCKET nfds;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  ClientData_ptr client, nextClient;

  while (processLife) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE) {
      if (!GetMessage(&msg, NULL, 0, 0))
        break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exc_set);

    FD_SET(serverSocket, &read_set);
    nfds = serverSocket;

    // max socket
    LIST_WHILE(clientList, client, next_client, next);
    if (client->sock > nfds)
      nfds = client->sock;
    FD_SET(client->sock, &read_set);
    FD_SET(client->sock, &write_set);
    FD_SET(client->sock, &exc_set);
    LIST_WHILEEND(clientList, client, next_client);

    if (select(nfds + 1, &read_set, &write_set, &exc_set, &tv) < -1) {
      log("select Error!\r\n");
      continue;
    }

    // new connect request
    if ()
  }
}