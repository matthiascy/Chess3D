
// TCPServerGameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include"stdlib.h"
#include "TCPServerGame.h"
#include "TCPServerGameDlg.h"
/*#include "chess_network.h"*/

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SOCKET (WM_USER + 1) 
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

/////用户个数
UserInfo   userList[MAX_LISTEN];
int   userNum;
////在线用户个数
ClientInfo   OnClientList[MAX_LISTEN];
int   OnClientNum;
///匹配用户
MatchedUser   MatchList[MAX_LISTEN / 2];
int  MatchNum;

SOCKET conSock[MAX_LISTEN];

class CAboutDlg: public CDialog {
public:
  CAboutDlg();

  // 对话框数据
  enum { IDD = IDD_ABOUTBOX };

protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

  // 实现
protected:
  DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg(): CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTCPServerGameDlg 对话框




CTCPServerGameDlg::CTCPServerGameDlg(CWnd* pParent /*=NULL*/)
  : CDialog(CTCPServerGameDlg::IDD, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPServerGameDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  DDX_Control(pDX, IDC_LIST1, m_serverList);
}

BEGIN_MESSAGE_MAP(CTCPServerGameDlg, CDialog)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  //}}AFX_MSG_MAP

  ON_BN_CLICKED(IDC_BUTTON1, &CTCPServerGameDlg::OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, &CTCPServerGameDlg::OnBnClickedButton2)

  ON_MESSAGE(WM_SOCKET, OnSocket)

END_MESSAGE_MAP()


// CTCPServerGameDlg 消息处理程序

BOOL CTCPServerGameDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  // 将“关于...”菜单项添加到系统菜单中。

  // IDM_ABOUTBOX 必须在系统命令范围内。
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != NULL) {
    BOOL bNameValid;
    CString strAboutMenu;
    bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
    ASSERT(bNameValid);
    if (!strAboutMenu.IsEmpty()) {
      pSysMenu->AppendMenu(MF_SEPARATOR);
      pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
    }
  }

  for (int i = 0; i < MAX_LISTEN; i++) {
    conSock[i] = INVALID_SOCKET;
    memset(userList, 0, sizeof(userList));
  }

  // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
  //  执行此操作
  SetIcon(m_hIcon, TRUE);			// 设置大图标
  SetIcon(m_hIcon, FALSE);		// 设置小图标

  // TODO: 在此添加额外的初始化代码
  cnnNum = 0;  //初始化全局变量
  for (int i = 0; i < MAX_LISTEN; i++) {
    OnClientList[i].clientSock = INVALID_SOCKET;
  }
  return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTCPServerGameDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
  if ((nID & 0xFFF0) == IDM_ABOUTBOX) {
    CAboutDlg dlgAbout;
    dlgAbout.DoModal();
  } else {
    CDialog::OnSysCommand(nID, lParam);
  }
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTCPServerGameDlg::OnPaint()
{
  if (IsIconic()) {
    CPaintDC dc(this); // 用于绘制的设备上下文

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // 使图标在工作区矩形中居中
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // 绘制图标
    dc.DrawIcon(x, y, m_hIcon);
  } else {
    CDialog::OnPaint();
  }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTCPServerGameDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////创建绑定监听套接字//////////////////////////////////////
SOCKET CreateServerSocket(const char* pszIP, short int nPort)
{
  SOCKET server = socket(AF_INET, SOCK_STREAM, 0);//创建服务器本地套接字，并进行错误检测
  if (server == INVALID_SOCKET) {
    printf("socket() error: %d\n", WSAGetLastError());
    return INVALID_SOCKET;
  }

  //创建本地实体
  sockaddr_in  local;
  memset(&local, 0, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = (
    pszIP != NULL ? inet_addr(pszIP) : INADDR_ANY);
  local.sin_port = htons(nPort);    //端口号

  //将服务器本地套接字和本地实体绑定
  if (bind(server, (struct sockaddr *)&local, sizeof(local)) != 0) {
    printf("bind() error: %d\n", WSAGetLastError());
    closesocket(server);
    return INVALID_SOCKET;
  }
  //listen进行监听本地套接字
  listen(server, 5);
  return server;
}


bool flag1 = true; //全局变量
void CTCPServerGameDlg::OnBnClickedButton1()  //开启服务器
{
  // TODO: 在此添加控件通知处理程序代码
  if (flag1 == true) {
    sockSvr = CreateServerSocket(NULL, SERVER_PORT);//端口号为5050  初始化 绑定 本地套接字 以及监听 过程
    ::WSAAsyncSelect(sockSvr, this->m_hWnd, WM_SOCKET,
                     FD_ACCEPT | FD_READ);	//使用多路复用模型进行TCP通信处理
    CTime t = CTime::GetCurrentTime();
    CString time = t.Format("%H:%M:%S");
    //time +="服务器启动成功!\r\n";
    time += "Server is starting!\r\n";
    m_serverList.AddString(time);//在用户状态栏显示服务器状态
    flag1 = false;
  } else {
    //MessageBox(L"服务已经开启！");
    MessageBox(L"Server is starting!");
  }
}

void CTCPServerGameDlg::OnBnClickedButton2()//关闭服务器
{
  // TODO: 在此添加控件通知处理程序代码
  if (flag1 == false) {
    closesocket(sockSvr);//关闭本地用于监听的套接字
    WSACleanup();//清除

    CTime t = CTime::GetCurrentTime();
    CString time = t.Format("%H:%M:%S");
    //time +="服务器关闭成功!\r\n";
    time += "Server close successfully!\r\n";
    m_serverList.AddString(time);
    //m_serverList.ResetContent();
    //m_serverList.ResetContent(); //清除在在线用户列表
    flag1 = true;
  } else {
    //MessageBox(L"服务已经关闭！");
    MessageBox(L"Server closed！");
  }
}




bool CTCPServerGameDlg::LoadPwdDict(const char* pszFileName)    //加载文件自带用户名与密码
{
  FILE* fp = fopen(pszFileName, "r");
  if (NULL == fp) {
    return false;
  }
  while (!feof(fp)) {
    UserInfo item;
    fscanf(fp, "%s%s", item.userName, item.userPwd);
    userList[userNum++] = item;
  }
  return true;
}

bool CTCPServerGameDlg::addUserDict(const char* pszFileName, const char* pszName, const char* pszPwd)
{
  FILE* fp = fopen(pszFileName, "a");//只写方式，数据追加在文件末尾
  if (NULL == fp) {
    return false;
  }
  //fprintf(fp,"%s%s",pszName,pszPwd);
  fprintf(fp, "\r\n%s%s", pszName, pszPwd);
  return true;
}

bool CTCPServerGameDlg::isValidUser(const char* pszName, const char* pszPwd)
//////用户有效则返回true 否则返回false
{
  for (int i = 0; i < userNum; i++) {
    /*
    if (strcmp(userList[i].userName, pszName) == 0 &&
    strcmp(userList[i].userPwd, pszPwd) == 0) {
    return true;
    }
    */
    if (strcmp(userList[i].userName, pszName) == 0) {
      if (strcmp(userList[i].userPwd, pszPwd) == 0) {
        return true;
      } else {
        return false;
      }
    }
  }
  return true;
}

void CTCPServerGameDlg::addUser(const char* pszName, const char* pwd)
{
  /*
  strcpy(OnClientList[OnClientNum].client.userName, pszName);
  ++OnClientNum;
  */
  for (int i = 0; i < MAX_LISTEN; i++) {
    if (userList[i].userName != "0") {
      strcpy_s(userList[i].userName, pszName);
      strcpy_s(userList[i].userPwd, pwd);
      userNum++;
    }
  }
}

void CTCPServerGameDlg::deleteUser(const char* pszName)
{
  for (int i = 0; i < OnClientNum; i++) {
    if (strcmp(OnClientList[OnClientNum].client.userName, pszName) == 0) {
      OnClientNum--;
      while (i < OnClientNum) {
        OnClientList[i] = OnClientList[i + 1];
        i++;
      }
    }
  }
}

void CTCPServerGameDlg::addClient(const char* name, SOCKET s)
{
  for (int i = 0; i < MAX_LISTEN; i++) {
    if (OnClientList[i].clientSock == INVALID_SOCKET) {
      for (int j = 0; j < userNum; j++) {
        if (strcmp(userList[j].userName, name) == 0) {
          OnClientList[i].client = userList[i];
          OnClientList[i].clientSock = s;
          OnClientList[i].clientState = NT_NULL;
        }
      }
    }
  }
}


void CTCPServerGameDlg::messageProcessing(const char* recvBuff, SOCKET s)
{
  LoadPwdDict("User.txt");
  PacketHeader* PktHeader = (PacketHeader*)recvBuff;

  if (PktHeader->packetType == PKTMSG) {
    switch (PktHeader->msgType) {
      case MSGLOGIN: {
        //如果PacketMessage结构体作为login包结构，则message存储的为用户密码

        PacketMessage* packet = (PacketMessage*)recvBuff;
        if (isValidUser(packet->name, packet->message)) {
          addUserDict("User.txt", packet->name, packet->message);
          addUser(packet->name, packet->message);
          addClient(packet->name, s);
          //send
          sendPacket(packet->name, MSGLOGIN, NULL, NT_NULL);


          cnnNum = cnnNum + 1;
          char t[100];
          itoa(cnnNum, t, 10);
          strcat(t, "client(s) online");
          strcat(packet->message, "LOGIN");
          m_serverList.AddString((LPCTSTR)packet->message);
          m_serverList.AddString((LPCTSTR)t);
        }
        break;
      }

      case MSGLOGOUT: {
        PacketMessage* packet = (PacketMessage*)recvBuff;
        deleteUser(packet->name);

        cnnNum = cnnNum - 1;
        char t[100];
        itoa(cnnNum, t, 10);
        strcat(t, "client(s) online");
        strcat(packet->message, "LOGOUT");
        m_serverList.AddString((LPCTSTR)packet->message);
        m_serverList.AddString((LPCTSTR)t);
        break;
      }

      case MSGCHAT: {
        break;
      }

      case MSGMATCH: {
        PacketMessage* packet = (PacketMessage*)recvBuff;

        if (packet->state == NT_SEARCHGAME) { //如果包中含有搜寻匹配的状态
          for (int i = 0; i < OnClientNum; i++) {   //1
            if (strcmp(OnClientList[i].client.userName, packet->name) == 0) {//2
              OnClientList[i].clientState = NT_SEARCHGAME;
              for (int j = 0; j < OnClientNum; j++) {
                if (OnClientList[j].clientState = NT_SEARCHGAME &&
                    strcmp(OnClientList[j].client.userName, packet->name) != 0) {
                  for (int k = 0; k < MAX_LISTEN / 2; k++) { //匹配数组
                    if (MatchList[k].connectedUser[0].client.userName == NULL
                        &&MatchList[k].connectedUser[1].client.userName == NULL) {
                      strcpy(MatchList[k].connectedUser[0].client.userName, OnClientList[i].client.userName);
                      strcpy(MatchList[k].connectedUser[1].client.userName, OnClientList[j].client.userName);
                      //       MatchList[k].connectedUser[1].client.userName = OnClientList[j].client.userName;
                      MatchList[k].flag = false;
                    }
                  }/////////////////////////////////找到了都在搜索匹配状态的另一用户，并将两个用户预先存储在匹配数组中

                  PacketMessage  item;
                  item.header.packetType = PKTMSG;
                  item.header.msgType = MSGMATCH;
                  item.state = NT_CONFIREMATCH;
                  ::send(OnClientList[i].clientSock, (char*)&item, sizeof(PacketMessage), 0);
                  ::send(OnClientList[j].clientSock, (char*)&item, sizeof(PacketMessage), 0);
                } else {   //没找到都在搜索匹配状态的另一用户
                  PacketMessage  item;
                  item.header.packetType = PKTMSG;
                  item.header.msgType = MSGMATCH;
                  item.state = NT_NULL;
                  ::send(s, (char*)&item, sizeof(PacketMessage), 0);
                }
              }
            } else {        //没在在线用户表中找到自己的
              PacketMessage  item;
              item.header.packetType = PKTMSG;
              item.header.msgType = MSGMATCH;
              item.state = NULL;
              ::send(s, (char*)&item, sizeof(PacketMessage), 0);
            }
          }//1
        }

        if (packet->state == NT_CONFIREMATCH) { /////如果包中含有确认匹配的状态
          for (int m = 0; m < OnClientNum; m++) { //把在用户列表中的该连接用户状态置为确认连接状态
            if (strcmp(OnClientList[m].client.userName, packet->name) == 0) {
              OnClientList[m].clientState = NT_CONFIREMATCH;
            }
          }
        }

        for (int i = 0; i < MAX_LISTEN / 2; i++) {
          if (strcmp(MatchList[i].connectedUser[0].client.userName, packet->name) == 0 ||
              strcmp(MatchList[i].connectedUser[1].client.userName, packet->name) == 0) {
            if (MatchList[i].connectedUser[0].clientState == NT_CONFIREMATCH &&
                MatchList[i].connectedUser[1].clientState == NT_CONFIREMATCH) {
              MatchList[i].flag = true;
              // send
              sendPacket(MatchList[i].connectedUser[0].client.userName, MSGMATCH, NULL, NT_CONFIREMATCH);
              sendPacket(MatchList[i].connectedUser[1].client.userName, MSGMATCH, NULL, NT_CONFIREMATCH);
            }
          }
        }
        /*
                  for (int k = 0; k < MAX_LISTEN / 2; k++) {//////////////////q
                  if (strcmp(MatchList[k].connectedUser[0].client.userName, packet->name) == 0 ||
                  strcmp(MatchList[k].connectedUser[1].client.userName, packet->name) == 0) {/////ast
                  if (strcmp(MatchList[k].connectedUser[0].client.userName, packet->name) == 0 &&
                  strcmp(MatchList[k].connectedUser[1].client.userName, packet->name) != 0) {//u
                  for (int r; r < OnClientNum; r++) {//p
                  if (strcmp(OnClientList[r].client.userName, MatchList[k].connectedUser[1].client.userName) == 0
                  && OnClientList[r].clientState == MatchList[k].connectedUser[0].clientState) {//k
                  MatchList[k].flag = true;
                  // OnClientList[m].clientState = NT_CONFIREMATCH;
                  for (int n = 0; n < OnClientNum; n++) {
                  if (strcmp(OnClientList[n].client.userName, MatchList[k].connectedUser[0].client.userName) == 0
                  || strcmp(OnClientList[n].client.userName, MatchList[k].connectedUser[1].client.userName) == 0) {
                  OnClientList[n].clientState = NT_INGAME;
                  }
                  }

                  PacketMessage  item, itema;
                  item.header.packetType = PKTMSG;
                  itema.header.packetType = PKTMSG;

                  item.header.msgType = MSGMATCH;
                  itema.header.msgType = MSGMATCH;

                  item.state = BLACK;
                  itema.state = WHITE;
                  ::send(MatchList[k].connectedUser[0].clientSock, (char*)&item, sizeof(PacketMessage), 0);
                  ::send(MatchList[k].connectedUser[1].clientSock, (char*)&itema, sizeof(PacketMessage), 0);

                  }//k
                  }//p
                  } else {
                  PacketMessage  item;
                  item.header.packetType = PKTMSG;
                  item.header.msgType = MSGMATCH;
                  item.state = NT_NULL;
                  ::send(s, (char*)&item, sizeof(PacketMessage), 0);
                  }
                  if (strcmp(MatchList[k].connectedUser[1].client.userName, packet->name) == 0
                  && strcmp(MatchList[k].connectedUser[0].client.userName, packet->name) != 0) {
                  for (int r; r < OnClientNum; r++) {//p
                  if (strcmp(OnClientList[r].client.userName, MatchList[k].connectedUser[0].client.userName) == 0
                  && OnClientList[r].clientState == MatchList[k].connectedUser[1].clientState) {//k
                  MatchList[k].flag = true;
                  // OnClientList[m].clientState = NT_CONFIREMATCH;
                  for (int n = 0; n < OnClientNum; n++) {
                  if (strcmp(OnClientList[n].client.userName, MatchList[k].connectedUser[0].client.userName) == 0
                  || strcmp(OnClientList[n].client.userName, MatchList[k].connectedUser[1].client.userName) == 0) {
                  OnClientList[n].clientState = NT_INGAME;
                  }
                  }

                  PacketMessage  item, itema;
                  item.header.packetType = PKTMSG;
                  itema.header.packetType = PKTMSG;

                  item.header.msgType = MSGMATCH;
                  itema.header.msgType = MSGMATCH;

                  item.state = BLACK;
                  itema.state = WHITE;
                  ::send(MatchList[k].connectedUser[0].clientSock, (char*)&item, sizeof(PacketMessage), 0);
                  ::send(MatchList[k].connectedUser[1].clientSock, (char*)&itema, sizeof(PacketMessage), 0);
                  }//k
                  }//p
                  } else {
                  PacketMessage  item;
                  item.header.packetType = PKTMSG;
                  item.header.msgType = MSGMATCH;
                  item.state = NT_NULL;
                  ::send(s, (char*)&item, sizeof(PacketMessage), 0);
                  }
                  } else {
                  PacketMessage  item;
                  item.header.packetType = PKTMSG;
                  item.header.msgType = MSGMATCH;
                  item.state = NT_NULL;
                  ::send(s, (char*)&item, sizeof(PacketMessage), 0);
                  }////////////////ast
                  }
                  }

                  }break;

                  }
                  }
                  */

        if (PktHeader->packetType == PKTGAME) {
          PacketGame* pktGame = (PacketGame*)recvBuff;
          for (int i = 0; i < MatchNum; i++) {
            //while (MatchList[i].flag == true) {
            if (MatchList[i].flag == true) {
              if (strcmp(MatchList[i].connectedUser[0].client.userName, pktGame->name) == 0) {
                ::send(MatchList[i].connectedUser[1].clientSock, recvBuff, sizeof(PacketGame), 0);
              } else {
                ::send(MatchList[i].connectedUser[0].clientSock, recvBuff, sizeof(PacketGame), 0);
              }
            }
          }
        }
      }
    }
  }
}

void CTCPServerGameDlg::sendPacket(const char* name, MSGTYPE msgType,
                                   char* msg, char state)
{
  PacketMessage packet;
  memset(&packet, 0, sizeof(packet));
  packet.header.packetType = PKTMSG;
  packet.header.msgType = msgType;
  strcpy_s(packet.name, name);
  strcpy_s(packet.message, msg);
  packet.state = state;
  for (int i = 0; i < OnClientNum; i++) {
    if (strcmp(OnClientList[i].client.userName, name) == 0) {
      ::send(OnClientList[i].clientSock, (char*)&packet, sizeof(packet), 0);
    }
  }
}

LRESULT CTCPServerGameDlg::OnSocket(WPARAM wParam, LPARAM lParam)//消息处理函数
{
  char szBuffer[MAX_BUFFER_SIZE] = { 0 };
  switch (WSAGETSELECTEVENT(lParam)) {
    case FD_ACCEPT:
    {
      int fromLen = sizeof(fromAddr);
      for (int i = 0; i < MAX_LISTEN; i++) {
        if (conSock[i] == INVALID_SOCKET) {
          conSock[i] = ::accept(sockSvr, (sockaddr *)&fromAddr, &fromLen);
          WSAAsyncSelect(conSock[i], this->m_hWnd, WM_SOCKET,
                         FD_WRITE | FD_CLOSE);
        }
        break;
      }
      //sockCnnn = ::accept(sockSvr, (sockaddr *)&fromAddr, &fromLen);
      //WSAAsyncSelect(sockCnnn, m_hWnd, WM_SOCKET, FD_READ);
      MessageBox(L"nihao！");
      return true;
      break;
    }

    case FD_READ: {
      for (int i = 0; i < MAX_LISTEN; i++) {
        /*
        if (OnClientList[i].clientSock = INVALID_SOCKET) {
        ::recv(OnClientList[i].clientSock, szBuffer, 1024, 0);
        messageProcessing(szBuffer, OnClientList[i].clientSock);
        }
        */
        if (conSock[i] == wParam) {
          ::recv(conSock[i], szBuffer, MAX_BUFFER_SIZE, 0);
          messageProcessing(szBuffer, conSock[i]);
        }
      }
      break;
    }
  }
  return 0;
}


