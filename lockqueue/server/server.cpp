#include <stdio.h>
#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")
#include <vector>
#include "packet.h"
#include <conio.h>
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
	clientService.sin_port = htons( 9001 );

	
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

	int a[MAX_SOCKS]={0};
	int rp[MAX_SOCKS]={0};
	char rbuf[MAX_SOCKS][1000]={0};
	SOCKET ConnectSocket = socks[0];
	Packet pk;
	pk.head_.op = op_addserver;
	pk.head_.len = 4;
	pk.buf_.resize(4);

	int server_id = 1;
	while(1)
	{
		if (_kbhit())
		{
			server_id = _getch() - 48;
			Sleep(1);
			break;
		}
	}
	//memcpy(&pk.buf_[0], &server_id, 4);
	send(ConnectSocket, (const char*)&pk.head_,sizeof(Head), 0);
	send(ConnectSocket, (const char*)&server_id, 4, 0);
	while(1)
	{
		for (int i=0; i< socks.size(); i++)
		{
			int rd = recv(ConnectSocket,&rbuf[i][rp[i]], 1000 - rp[i],0);
			if (rd > 0)
			{
				rp[i] += rd;
			}else if(rd < 0)
			{
				printf("recv error %d",GetLastError());
				getchar();
			}

			while(rp[i] >= sizeof(Head))
			{
				Head* head = (Head*)rbuf[i];
				if (rp[i] >= sizeof(Head) + head->len)
				{
					if (head->op == op_jump)
					{
						head->op = op_jump_success;
					}
					int c;
					memcpy(&c, rbuf[i] + sizeof(Head), head->len);
					rp[i] -= sizeof(Head);
					rp[i] -= head->len;
					send(ConnectSocket, &rbuf[i][0], sizeof(Head)+head->len, 0);

					memmove(rbuf[i],&rbuf[i][sizeof(Head)+head->len], rp[i]);
					printf("recv %d %d %d %d", i, head->op, head->guid, c);
					
				}
			}
		}
	Sleep(1);
	}
	WSACleanup();
	return;
}

