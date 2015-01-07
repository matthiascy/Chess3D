#pragma comment(lib, "ws2_32.lib")
#include "chess_client.h"

ChessClient::ChessClient()
{
  WSADATA wsaData;
  WSAStartup(MAKEWORD(1, 0), &wsaData);
}

ChessClient::~ChessClient()
{
  WSACleanup();
}

void ChessClient::initialize()
{
  clientSock = INVALID_SOCKET;
  memset(&recvBuff, 0, sizeof(recvBuff));
  memset(&sendBuff, 0, sizeof(sendBuff));
  isConnected = false;
}

bool ChessClient::connectToServer(const char* ip, short int port)
{
  SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
  if (clientSock == INVALID_SOCKET) {
    return false;
  }

  sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(sockaddr_in));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
  serverAddr.sin_port = htons(port);
  if (connect(clientSock, (sockaddr*)&serverAddr,
    sizeof(serverAddr)) != 0) {
    closesocket(clientSock);
    return false;
  }

  return true;
}

void ChessClient::sendMessage(PACKETTYPE type, MSGTYPE msgType, char state,
                              char* msg, float x/* = 0*/, float y/* = 0*/)
{
  if (isConnected) {
    switch (type) {
      case PKTMSG: {
        PacketMessage packet;
        memset(&packet, 0, sizeof(packet));
        packet.header.packetType = PKTMSG;
        packet.header.msgType = msgType;
        packet.header.state = state;
        strcpy_s(packet.name, this->name);

        send(this->clientSock, (char*)&packet, sizeof(packet), 0);
        break;
      }

      case PKTGAME: {
        PacketGame packet;
        memset(&packet, 0, sizeof(packet));
        packet.header.packetType = PKTGAME;
        packet.header.state = state;
        strcpy_s(packet.name, this->name);
        packet.pos.x = x;
        packet.pos.y = y;

        send(this->clientSock, (char*)&packet, sizeof(packet), 0);
        break;
      }
    }
  }
}

void ChessClient::recvMessage()
{
  recv(clientSock, recvBuff, MAX_BUFFER_SIZE, 0);
}

void ChessClient::processPacket()
{
  PacketHeader* header = (PacketHeader*)recvBuff;
  switch (header->packetType) {
    case PKTGAME: {
      PacketGame* packet = (PacketGame*)recvBuff;
      /*
      this->chessPos.oriCol = packet->pos.oriCol;
      this->chessPos.oriRow = packet->pos.oriRow;
      this->chessPos.desCol = packet->pos.desCol;
      this->chessPos.desRow = packet->pos.desRow;
      */
      this->chessPos.x = packet->pos.x;
      this->chessPos.y = packet->pos.y;
      break;
    }

    case PKTMSG: {
      PacketMessage* packet = (PacketMessage*)recvBuff;
      switch (packet->header.msgType) {
        case MSGSTATE: {
          this->gameState = packet->header.state;
          //MessageBox(NULL, packet->message, "Message", MB_OK);
          break;
        }

        case MSGLOGIN:
        case MSGLOGOUT:
        case MSGCHAT:
          break;
      }
      break;
    }
  }
}