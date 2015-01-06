#ifndef __NETWORK_H__
#define __NETWORK_H__

// set socket to non-block mode
void nonblock(SOCKET s);

// initialize server socket
SOCKET initServerSock(int port, int backlog);

// new connection request process
void acceptNewClient(SOCKET motherSocket);

// disconnect from server
void disconnectClient(ClientData_ptr client);

// copy data to buffer
void sendData(ClientData_ptr client, const char* data, int size);

// send message to all connected client
void sendToAll(const char* data, int size);

// clean sending buffer
int flushSendByff(ClientData_ptr client);

// recv
bool recvFromClient(ClientData_ptr client);

#endif
