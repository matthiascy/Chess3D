#ifndef __CHESS_NETWORK_H__
#define __CHESS_NETWORK_H__

#define SERVER_PORT    1994
#define MAX_LISTEN     200

#define MAX_BUFFER_SIZE 1024
#define MAX_MSG_SIZE    512
#define STR_LEN         32

const char NT_SEARCHGAME = 0x20;
const char NT_CONFIREMATCH = 0x21;
const char NT_INGAME = 0x22;
const char NT_LOGIN = 0x23;
const char NT_COLOR = 0x24;
const char NT_NULL = 0x00;

const char WHITE	= 0x01;
const char BLACK	= 0xFE;

const char login[STR_LEN] = "login_successfully";


SOCKET CreateServerSocket(const char* pszIP, short int nPort); //创建本地套接字 绑定 监听 函数

enum PACKETTYPE {
	PKTMSG, PKTGAME
};

enum MSGTYPE {
	MSGLOGIN, MSGLOGOUT, MSGCOLOR, MSGCHAT, MSGMATCH, MSGNULL
};

typedef struct Position {
	float x, y;
} Position;

typedef struct PacketHeader {
	PACKETTYPE packetType;
	MSGTYPE msgType;
} PacketHeader;

typedef struct PacketMessage {
	PacketHeader header;
	char name[STR_LEN];
	char message[STR_LEN];
	char state;
} PacketMessage;

typedef struct PacketGame {
	PacketHeader header;
	char name[STR_LEN];
	char color;
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
	bool flag;
} MatchedUser;

#endif // __CHESS_NETWORK_H__