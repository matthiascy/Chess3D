#ifndef __CHESS_NETWORK_H__
#define __CHESS_NETWORK_H__

#define SERVER_PORT    1994
#define MAX_LISTEN     200

#define MAX_BUFFER_SIZE 1024
#define MAX_MSG_SIZE    512
#define STR_LEN         32

enum PACKETTYPE {
  PKTMSG, PKTGAME
};

typedef struct Postion {
  int oriCol;
  int oriRow;
  int desCol;
  int desRow;
} Position;

typedef struct PacketHeader {
  PACKETTYPE packetType;
} PacketHeader;

typedef struct PacketMessage {
  PacketHeader header;
  char name[STR_LEN];
  char message[STR_LEN];
} PacketMessage;

typedef struct PacketGame {
  PacketHeader header;
  char name[STR_LEN];
  Postion pos;
} PacketGame;

#endif // __CHESS_NETWORK_H__