#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define PACKET_ACTIVE_CHECK 0
#define PACKET_CHANGE_NICK  1
#define PACKET_RECV_MSG     2  // message, client to server
#define PACKET_SEND_MSG     3  // message, server to client

bool readRecvBuff(ClientData_ptr client);

#endif
