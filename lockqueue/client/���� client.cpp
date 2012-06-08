#include <stdio.h>
#include "winsock2.h"
#pragma comment(lib, "Ws2_32.lib")
#include <vector>
#include "packet.h"
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
#define MAX_SOCKS	100
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
	int aa[MAX_SOCKS]={0};
	int aat[MAX_SOCKS]={0};
	for (int i=0; i<MAX_SOCKS; i++)
	{
		aat[i]=GetTickCount();
	}
	while(1)
	{
		for (int i=0; i< socks.size(); i++)
		{
			SOCKET ConnectSocket = socks[i];

			Packet pk;
			pk.head_.op = i;
			pk.head_.len = 4;
			pk.buf_.resize(4);
			a[i]++;
			memcpy(&pk.buf_[0], &a, sizeof(a[i]));
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
			//printf("send %d %d",i, a[i]);
			if (1)
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
						int c;
						memcpy(&c, rbuf[i] + sizeof(Head), head->len);
						rp[i] -= sizeof(Head);
						rp[i] -= head->len;
						memmove(rbuf[i],&rbuf[i][sizeof(Head)+head->len], rp[i]);
						//printf("recv %d %d ", i, c);
					}
				}
				if (GetTickCount() - aat[i] > 1000)
				{
					aat[i] = GetTickCount();
					int j = a[i] - aa[i];
					printf("recv %d %d ", i, j);
					aa[i] = a[i];
				}
			}

		}
		
	//Sleep(1000);
	}
	WSACleanup();
	return;
}

