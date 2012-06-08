#include <stdio.h>
#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")
#include <vector>
#include "packet.h"
#include <conio.h>
#include <iostream>
void main() {
	//----------------------
	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != NO_ERROR)
		printf("Error at WSAStartup()\n");

	hostent *dd = gethostbyname("www.conngame.com");

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	sockaddr_in clientService; 
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	clientService.sin_port = htons( 9000 );

	
	//----------------------
	// Connect to server.
#define MAX_SOCKS	1
	std::vector<SOCKET> socks;
	for (int i=0; i<MAX_SOCKS; i++)
	{
		SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
		if ( connect( ConnectSocket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) {
			return ;
			WSACleanup();
		}
		socks.push_back(ConnectSocket);
	}


	int rp={0};
	char rbuf[1000]={0};
	int a = 0;
	int client_id = 10;
	int server_id = 0;

	SOCKET ConnectSocket = socks[0];
	bool enterserver =false;
	while(1)
	{
		if (_kbhit())
		{
			if (_getch() == '1')
			{
				//std::cin>>client_id;
				Packet pk;
				pk.head_.op = op_jump;
				pk.head_.len = 4;
				pk.head_.guid = client_id;
				pk.buf_.resize(4);
				server_id = 1;
				memcpy(&pk.buf_[0], &server_id, 4);
				int r=0;
				r = send(ConnectSocket, (const char *)&pk.head_, sizeof(pk.head_), 0);
				if (r < 0)
				{
					printf("error1 %d",GetLastError());
					getchar();
				}
				r = send(ConnectSocket, &pk.buf_[0], pk.buf_.size(), 0);
				if (r < 0)
				{
					printf("error2 %d",GetLastError());
					getchar();
				}
				printf("jump 1 server");
				r = recv(ConnectSocket, &rbuf[rp], 1000-rp, 0);
				if (r > 0)
				{
					rp += r;
					while (rp > sizeof(Head))
					{
						Head* head = (Head*)rbuf;
						if (rp >= sizeof(Head) + head->len)
						{
							if (head->op == op_jump_success)
							{
								int c;
								memcpy(&c, rbuf + sizeof(Head), head->len);
								rp -= sizeof(Head);
								rp -= head->len;
								memmove(rbuf,&rbuf[sizeof(Head)+head->len], rp);
								printf("jump %d server success", c);
								enterserver = true;
							}else
							{
								int c;
								memcpy(&c, rbuf + sizeof(Head), head->len);
								rp -= sizeof(Head);
								rp -= head->len;
								memmove(rbuf,&rbuf[sizeof(Head)+head->len], rp);
							}

						}
					}
				}
				
			}else if (_getch() == '2')
			{
				//std::cin>>client_id;
				Packet pk;
				pk.head_.op = op_jump;
				pk.head_.len = 4;
				pk.head_.guid = client_id;
				pk.buf_.resize(4);
				server_id = 2;
				memcpy(&pk.buf_[0], &server_id, 4);
				int r=0;
				r = send(ConnectSocket, (const char *)&pk.head_, sizeof(pk.head_), 0);
				if (r < 0)
				{
					printf("error1 %d",GetLastError());
					getchar();
				}
				r = send(ConnectSocket, &pk.buf_[0], pk.buf_.size(), 0);
				if (r < 0)
				{
					printf("error2 %d",GetLastError());
					getchar();
				}
				printf("jump 2 server");
				r = recv(ConnectSocket, &rbuf[rp], 1000-rp, 0);
				if (r > 0)
				{
					rp += r;
					while (rp > sizeof(Head))
					{
						Head* head = (Head*)rbuf;
						if (rp >= sizeof(Head) + head->len)
						{
							if (head->op == op_jump_success)
							{
								int c;
								memcpy(&c, rbuf + sizeof(Head), head->len);
								rp -= sizeof(Head);
								rp -= head->len;
								memmove(rbuf,&rbuf[sizeof(Head)+head->len], rp);
								printf("jump %d server success", c);
								enterserver = true;
							}else
							{
								int c;
								memcpy(&c, rbuf + sizeof(Head), head->len);
								rp -= sizeof(Head);
								rp -= head->len;
								memmove(rbuf,&rbuf[sizeof(Head)+head->len], rp);
							}

						}
					}
				}
			}
		}else
		{
			if (!enterserver)
			{
				continue;
			}

			Packet pk;
			pk.head_.op = op_move;
			pk.head_.len = 4;
			pk.buf_.resize(4);
			a++;
			memcpy(&pk.buf_[0], &a, 4);
			int r=0;
			r = send(ConnectSocket, (const char *)&pk.head_, sizeof(pk.head_), 0);
			if (r < 0)
			{
				printf("error1 %d",GetLastError());
				getchar();
			}
			r = send(ConnectSocket, &pk.buf_[0], pk.buf_.size(), 0);
			if (r < 0)
			{
				printf("error2 %d",GetLastError());
				getchar();
			}
	
			int rd = recv(ConnectSocket,&rbuf[rp], 1000 - rp,0);
			if (rd > 0)
			{
				rp += rd;
			}else if(rd < 0)
			{
				printf("recv error %d",GetLastError());
				getchar();
			}
		}
				
		Sleep(1000);
	}
	WSACleanup();
	return;
}

