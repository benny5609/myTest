#ifndef _ACCEPT_SOCKET_H_
#define _ACCEPT_SOCKET_H_
#include "common.h"
static int num = 0;
class AcceptSocket
{
public:
	int init()
	{
		WSAData data;
		WSAStartup(MAKEWORD(2,2), &data);
		int r = 0;
		r = init_client();
		if (r < 0)
		{
			if (client_socket_ != INVALID_SOCKET)
			{
				closesocket(client_socket_);
			}
			WSACleanup();
		}
		r = init_server();
		if (r < 0)
		{
			if (client_socket_ != INVALID_SOCKET)
			{
				closesocket(client_socket_);
			}
			if (server_socket_ != INVALID_SOCKET)
			{
				closesocket(server_socket_);
			}
			WSACleanup();
		}

		::_beginthread(AcceptSocket::run, 0, this);

		return 0;
	}
	int init_client()
	{
		client_socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if (client_socket_ == INVALID_SOCKET)
		{
			return -1;
		}
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9000);
		addr.sin_addr.s_addr = ADDR_ANY;
		if(bind(client_socket_, (SOCKADDR*)(&addr), sizeof(addr)) == SOCKET_ERROR)
		{
			return -1;
		}

		if(listen(client_socket_, 20) == SOCKET_ERROR)
		{
			return -1;
		}
		return 0;
	}

	int init_server()
	{
		server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
		if (server_socket_ == INVALID_SOCKET)
		{
			return -1;
		}
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(9001);
		addr.sin_addr.s_addr = ADDR_ANY;
		if(bind(server_socket_, (SOCKADDR*)(&addr), sizeof(addr)) == SOCKET_ERROR)
		{
			return -1;
		}

		if(listen(server_socket_, 20) == SOCKET_ERROR)
		{
			return -1;
		}
		return 0;
	}

	static void run(void* p)
	{
		AcceptSocket *as = (AcceptSocket*)p;
		FD_SET rfd;
		FD_SET wfd;
		FD_SET efd;
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);
		FD_ZERO(&efd);
		while(1)
		{
			FD_SET(as->client_socket_,&rfd);
			FD_SET(as->server_socket_,&rfd);
			int r = select(0,&rfd, &wfd, &efd, NULL);
			if (r < 0)
			{
			}else if (r == 0)
			{

			}else
			{
				if (FD_ISSET(as->client_socket_, &rfd))
				{
					SOCKET s = accept(as->client_socket_, NULL, 0);
					if (s == INVALID_SOCKET)
					{
					}else
					{
						Packet msg;
						msg.head_.op = op_new;
						msg.head_.guid = s;
						g_clients.push(msg);
					}
				}else if (FD_ISSET(as->server_socket_, &rfd))
				{
					SOCKET s = accept(as->server_socket_, NULL, 0);
					if (s == INVALID_SOCKET)
					{
					}else
					{
						Packet msg;
						msg.head_.op = op_new;
						msg.head_.guid = s;
						g_server.push(msg);
					}
				}
			}

		}
	}
	SOCKET client_socket_;
	SOCKET server_socket_;
};
extern AcceptSocket g_accept;
#endif