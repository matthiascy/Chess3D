#include "file_define.h"
#include "structs.h"
#include "network.h"
#include "packet.h"
#include "linked_list.h"
#include "utils.h"
#include "message.h"

void packetAliveCheck(ClientDate_ptr client)
{
}

void packetChangeNick(ClientData_ptr client)
{
  char nick[64];
  
  getString(client->recvBUff, nick, client->recvPos);
  strcpy(client->name, nick);
}

void packetRecvMsg(ClientData_ptr client)
{
  char msg[MAX_BUFF];

  getString(client->recvBuff, msg, client->recvPos);

  int packetPos;
  char packet[MAX_BUFF];
  char tmp[MAX_BUFF];

  sprintf(tmp, "%s : %s \r\n", client->name, msg);

  packetPos = 2;
  putWord(packet, PACKET_SEND_MSG, packetPos);
  putString(packet, tmp, packetPos);
  setSize(packet, packetPos);

  sendToAll(packet, packePos);
}

bool readRecvBuff(ClientData_ptr client)
{
  unsigned short msgSize;
  unsigned short tag;

  while (client->recvSize > 0) {
    client->recvPos = 0;

    msgSize = getWord(client->recvBuff, client->recvPos);
    tag = getWOrd(client->recvBuff, client->recvPos);

    // recvBuff's data size < pakcet size
    if (client->recvSize < msgSize)
      return 1;

    // packet process
    switch (tag) {
    case PACKET_ALIVE_CHECK:
      packetAliveCheck(client);
      break;

    case PACKET_CHANGE_NICK:
      packetChangeNick(client);
      break;

    case PACKET_RECV_MSG:
      packetRecvMsg(client);
      break;

    case PACKET_SEND_MSG:
      break;
    }

    memmove(&client->recvBuff[0], &client->recvBuff[msgSize],
	    client->recvSize - msgSize);

    client->recvSize -= msgSize;
  }
  return true;
}
