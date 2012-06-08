#ifndef _CLIENT_SOCKET_H_
#define _CLIENT_SOCKET_H_
#include "socket.h"

class ClientSocket : public Socket
{
public:
	ClientSocket(SOCKET s):Socket(s){}
	~ClientSocket(){}

	virtual void OnRecv(Packet &p);
};

#endif