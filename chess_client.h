#ifndef __CHESS_CLIENT_H__
#define __CHESS_CLIENT_H__

#include <WinSock2.h>
#include "chess_network.h"

class ChessClient {
public:
  ChessClient();
  ~ChessClient();

  SOCKET getSocket() {
    return clientSock;
  };

  void setSocket(SOCKET s) {
    clientSock = s;
  };

  void setName(const char* name) {
    strcpy_s(this->name, name);
  };

  void setPwd(const char* pwd) {
    strcpy_s(this->pwd, pwd);
  };

  Position getPosition() {
    return testPos;
  };

  void setState(const char state) {
    gameState = state;
  };

  char getState() {
    return gameState;
  };

  void initialize();
  bool connectToServer(const char* ip, short int port);
  void sendMessage(PACKETTYPE type, MSGTYPE msgType, char state, char* msg, float x = 0, float y = 0);
  void recvMessage();
  void processPacket();

  char recvBuff[MAX_BUFFER_SIZE];
  char sendBuff[MAX_BUFFER_SIZE];

private:
  SOCKET clientSock;
  char name[STR_LEN];
  char pwd[STR_LEN];
  Position chessPos;
  Position testPos;
  bool isConnected;
  char gameState;
};

#endif // __CHESS_CLIENT_H__