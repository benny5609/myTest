#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "lock_queue.h"
#include "packet.h"

#define BUFF_SIZE 4096

class Socket
{
public:
	Socket(SOCKET s);
	virtual ~Socket() {}
	int update();
	int recv();
	int send();

	void close();
	void push(Packet& p);

	virtual void OnRecv(Packet &p) = 0;

	char send_buf_[BUFF_SIZE];
	int send_pos_;
	char recv_buf_[BUFF_SIZE];
	int recv_pos_;
	SOCKET sock_;
	std::queue<Packet> send_queue_;
};
#endif