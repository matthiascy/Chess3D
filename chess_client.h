#ifndef __CHESS_CLIENT_H__
#define __CHESS_CLIENT_H__

#include <WinSock2.h>
#include "chess_network.h"
#include "chess_board.h"

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
    return chessPos;
  };

  bool getConnection() {
    return this->isConnected;
  };

  void setConnection(bool state) {
    this->isConnected = state;
  };

  bool getIsGame() {
    return isGame;
  };

  char getClientColor() {
    return clientColor;
  }

  char getColor() {
    return color;
  };

  char* getPassword() {
    return pwd;
  };

  void initialize();
  bool connectToServer(const char* ip, short int port);
  // color used as color&NT_TYPE
  void sendMessage(PACKETTYPE type, MSGTYPE msgType,
                   char color, char* msg,
                   float x = 0.0f, float y = 0.0f);
  void recvMessage();
  void processPacket();

  char recvBuff[MAX_BUFFER_SIZE];
  char sendBuff[MAX_BUFFER_SIZE];

private:
  SOCKET clientSock;
  char name[STR_LEN];
  char pwd[STR_LEN];
  Position chessPos;
  bool isConnected;
  char clientColor;
  char color;
  bool isGame;
};

#endif // __CHESS_CLIENT_H__