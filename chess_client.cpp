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
  isGame = false;
  clientColor = NO_COLOR;
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

void ChessClient::sendMessage(PACKETTYPE type, MSGTYPE msgType, char color,
                              char* msg, float x/* = 0.0f*/, float y/* = 0.0f*/)
{
  if (isConnected) {
    switch (type) {
      case PKTMSG: {
        PacketMessage packet;
        memset(&packet, 0, sizeof(packet));
        packet.header.packetType = PKTMSG;
        packet.header.msgType = msgType;
        packet.state = color;
        strcpy_s(packet.name, this->name);

        send(this->clientSock, (char*)&packet, sizeof(packet), 0);
        break;
      }

      case PKTGAME: {
        PacketGame packet;
        memset(&packet, 0, sizeof(packet));
        packet.header.packetType = PKTGAME;
        packet.header.msgType = msgType;
        packet.color = color;
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
      this->chessPos.x = packet->pos.x;
      this->chessPos.y = packet->pos.y;
      this->color = packet->color;
      break;
    }

    case PKTMSG: {
      PacketMessage* packet = (PacketMessage*)recvBuff;
      switch (packet->header.msgType) {
        case MSGMATCH: {
          switch (packet->state) {
            case NT_CONFIREMATCH: {
              if (MessageBox(NULL, "Game found, enter?", "Confirm", MB_YESNO) == IDOK) {
                sendMessage(PKTMSG, MSGMATCH, NT_CONFIREMATCH, NULL);
              }
              break;
            }

            default:
              break;
          }
          break;
        }

        case MSGCOLOR: {
          clientColor = packet->state;
          isGame = true;
          break;
        }

        case MSGLOGIN:
        case MSGLOGOUT: {
          if (packet->state = NT_LOGIN) {
            isConnected = true;
          } else {
            MessageBox(NULL, "Connection failed!", "Info", MB_OK);
          }
          break;
        }

        case MSGCHAT: {
          MessageBox(NULL, packet->message, "Info", MB_OK);
          break;
        }
      }
      break;
    }
  }
}