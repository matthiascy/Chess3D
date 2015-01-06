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

  void setName(const char* name) {
    strcpy_s(this->name, name);
  };

  void setPwd(const char* pwd) {
    strcpy_s(this->pwd, pwd);
  }

  void initialize();
  bool connectToServer(const char* ip, short int port);
  void sendMessage(PACKETTYPE type, char* msg);
  void recvMessage();

private:
  SOCKET clientSock;
  char recvBuff[MAX_BUFFER_SIZE];
  char sendBuff[MAX_BUFFER_SIZE];
  char name[STR_LEN];
  char pwd[STR_LEN];
  Position chessPos;
};

#endif // __CHESS_CLIENT_H__