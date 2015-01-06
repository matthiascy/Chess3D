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

void ChessClient::sendMessage(PACKETTYPE type, char* msg)
{
  switch (type) {
    case PKTGAME: {
      PacketGame packet;
      packet.header.packetType = PKTGAME;
      strcpy_s(packet.name, name);
      packet.pos = chessPos;
      int ret = send(clientSock, (char*)&packet, sizeof(packet), 0);
      break;
    }

    case PKTMSG: {
      PacketMessage packet;
      packet.header.packetType = PKTMSG;
      strcpy_s(packet.name, name);
      strcpy_s(packet.message, msg);
      int ret = send(clientSock, (char*)&packet, sizeof(packet), 0);
      break;
    }
  }
}

void ChessClient::recvMessage()
{

}