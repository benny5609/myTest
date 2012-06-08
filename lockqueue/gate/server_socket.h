#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_
#include "socket.h"

class ServerSocket : public Socket
{
public:
	ServerSocket(SOCKET s):Socket(s){}
	~ServerSocket(){}

	virtual void OnRecv(Packet &p);
};

#endif