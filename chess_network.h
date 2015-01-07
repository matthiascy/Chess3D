#ifndef __CHESS_NETWORK_H__
#define __CHESS_NETWORK_H__

#define SERVER_PORT    1994
#define MAX_LISTEN     200

#define MAX_BUFFER_SIZE 1024
#define MAX_MSG_SIZE    512
#define STR_LEN         32

const char OPPSITE_STATE = 0x20;
const char NT_WAIT = 0x11;
const char NT_MOVING = 0x12;
const char NT_CAPTURE = 0x14;
const char NT_KILL = 0x18;
const char NT_MASK = 0x0F;
const char NT_SEARCHGAME = 0x20;
const char NT_INGAME = 0x21;

const char login[STR_LEN] = "login_successfully";

enum PACKETTYPE {
  PKTMSG, PKTGAME
};

enum MSGTYPE {
  MSGLOGIN, MSGLOGOUT, MSGSTATE, MSGCHAT, MSGMATCH, MSGNULL
};

typedef struct Position {
  float x, y;
} Position;

typedef struct PacketHeader {
  PACKETTYPE packetType;
  MSGTYPE msgType;
  char state;
} PacketHeader;

typedef struct PacketMessage {
  PacketHeader header;
  char name[STR_LEN];
  char message[STR_LEN];
} PacketMessage;

typedef struct PacketGame {
  PacketHeader header;
  char name[STR_LEN];
  Position pos;
} PacketGame;

typedef struct UserInfo {
  char userName[STR_LEN];
  char userPwd[STR_LEN];
} UserInfo;

typedef struct ClientInfo {
  UserInfo client;
  SOCKET clientSock;
  char clientState;
} ClientInfo;

typedef struct MatchedUser{
  ClientInfo connectedUser[2];
} MatchedUser;

#endif // __CHESS_NETWORK_H__