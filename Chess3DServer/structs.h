#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#define MAX_BUFF 1024
#define NAME_LEN 50
#define IP_LEN 20

typedef struct ClientData {
  SOCKET sock;
  char ip[IP_LEN];
  
  char recvBuff[MAX_BUFF];
  int recvSize;
  int recvPos;

  char sendBuff[MAX_BUFF];
  int sendSize;

  char name[NAME_LEN];

  unsigned long lastRecvTime;

  struct ClientData* prev;
  struct ClientData* next;
} ClientData, *ClientData_ptr;

#endif
