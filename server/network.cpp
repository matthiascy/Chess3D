#include "file_define.h"
#include "structs.h"
#include "linked_list.h"
#include "network.h"
#include "utils.h"

extern SOCKET serverSock;

extern ClientData_ptr clientList;

extern int totalClient;

#ifdef WIN32
void nonblock(SOCKET s)
{
  int mode = 1;
  ioctlsocket(s, FIONBIO, (unsigned long*)&mode);
}
#else
void nonblock(SOCKET s)
{
  int flags;

  flags = fcntl(s, F_GETFL, 0);
  flags |= O_NONBLOCK;
  if (fcntl(s, F_SETFL, flags) < 0)
    log("* non block Error\r\n");
}
#endif

SOCKET initServerSock(int port, int backlog)
{
  struct sockaddr_in svrAddr;
  SOCKET sock;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0) {
    log("initServerSock(), socket(...) failed [PORT:%d].. \r\n", port);
    return -1;
  }

#ifndef WIN32
  int opt = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
    log("* Error: sersockopt REUSEADDR\r\n");
    exit(1);
  }
#endif

  // LINGER settings
  struct linger ld;

  ld.l_onoff = 0;
  ld.l_linger = 0;
  if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&ld, sizeof(ld)) < 0)
    log("* Error: sersockopt SO_LINGER...\r\n");

  memset((char*)&svrAddr, 0, sizeof(svrAddr));

  svrAddr.sin_family = AF_INET;
  svrAddr.sin_port = htons(port);
  svrAddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr*)&svrAddr, sizeof(svrAddr)) < 0) {
    log("initServerSock(), bind(..) failed.. [PORT:%d]\r\n", port);
    closesocket(sock);
    return -1;
  }

  nonblock(sock);

  listen(sock, backlog);
  return sock;
}

// new connection process
void acceptNewClient(SOCKET motherSock)
{
  SOCKET newDesc;
  struct sockaddr_in peer;
  
#if defined WIN32
  int peersize;
#else
  socklen_t peersize;
#endif
  
  peersize = sizeof(peer);
  
  newDesc = accept(motherSock, (struct sockaddr*)&peer, &peersize);
  
  if (newDesc < 0) {
    log("Accept new client failed!\r\n");
    return;
  }
  
  nonblock(newDesc);
  ClientData_ptr newClient = (ClientData_ptr)malloc(sizeof(ClientData));
  
  if (!newClient) {
    closesocket(newDesc);
    return;
  }

  // client data init
  newClient->sock = newDesc;
  
  strcpy(newClient->ip, (char*)inet_ntoa(peer.sin_addr));
  
  *newClient->recvBuff = '\0';
  newClient->recvSize = 0;
  newClient->recvPos = 0;
  
  *newClient->sendBuff = '\0';
  newClient->sendSize = 0;
  
  *newClient->name = '\0';
  newClient->lastRecvTime = timeGetTime();
  
  newClient->prev = NULL;
  newClient->next = NULL;
  
  INSERT_TO_LIST(clientList, newClient, prev, next);
  totalClient++;
  
  log("Accept new connection: %d [%s]\r\n", newDesc, newClient->ip);
}

void sendData(ClientData_ptr client, const char* data, int size)
{
  if ((client->sendSize + size) > MAX_BUFF)
    return;
  
  memcpy(&client->sendBuff[client->sendSize], data, size);
  client->sendSize += size;
}

void sendToAll(const char* data, int size)
{
  ClientData_ptr client, next_client;

  LIST_WHILE(clientList, client, next_client, next);
  sendData(client, data, size);
  LIST_WHILEEND(clientList, client, next_client);
}

int flushSendBuff(ClientData_ptr client)
{
  int sendSize;
  sendSize = send(client->sock, client->sendBuff, client->sendSize, 0);

  if (sendSize <= 0)
    return -1;

  if (sendSize < client->sendSize) {
    memmove(&client->sendBuff[0], &client->sendBuff[sendSize],
            client->sendSize - sendSize);
    client->sendSize -= sendSize;
  } else {
    client->sendBuff = '\0';
    client->sendsize = 0;
  }
  return sendSize;
}

void disconnectClient(ClientData_ptr client)
{
  if (client->sock != INVALID_SOCKET)
    closesocket(client->sock);

  free(client);
  client = NULL;
}

bool recvFromClient(ClientData_ptr client)
{
  int recvSize;
  char recvBuff[MAX_BUFF];

  recvSize = recv(client->sock, recvBuff, 1024, 0);

  if (recvSize == 0)
    return false;
  
  if (recvSize < 0) {
#if defined WIN32
    if (WSAGetLastError() != WSAEWOULDBLOCK)
      return false;
#else
    if (errno != EWOULDBLOCK)
      return false;
#endif
    else
      return true;
  }
}
 
bool recvFromClient(ClientData_ptr client)
{
  int recvSize;
  char recvBuff[MAX_BUFF];

  recvSize = recv(client->sock, recvBuff, 1024, 0);

  if (recvSize == 0)
    return false;

  if (recvSize < 0) {
#if defined WIN32
    if (WSAGetLastError() != WSAWOULDBLOCK)
      return false;
#else
    if (errno != EWOULDBLOCK)
      return false;
#endif
    else
      return true;
  }

  // buffer overflow
  if ((client->recvSize + recvSize) >= MAX_SOCK_BUFF)
    return false;

  client->lastRecvTime = timeGetTime();

  memcpy(&client->recvBuff[client->recvSize], recvBuff, recvSize);
  client->recvSize += recvSize;

  return true;
}
