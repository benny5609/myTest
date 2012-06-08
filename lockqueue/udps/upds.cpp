#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")
#define BUF_MAX 64*1024
int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2,2), &data);

	SOCKET ls = socket(AF_INET, SOCK_DGRAM, 0);
	sockaddr_in laddr;
	memset(&laddr, 0, sizeof(laddr));
	laddr.sin_port = htons(9000);
	laddr.sin_addr.s_addr = ADDR_ANY;
	laddr.sin_family=AF_INET;
	bind(ls, (const sockaddr*)&laddr, sizeof(laddr));

	sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_port = htons(9001);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_family=AF_INET;
	connect(ls, (const sockaddr*)&saddr, sizeof(saddr));
	while(1)
	{
		char buf[BUF_MAX];
		int len = 0;
		sockaddr_in clientaddr;
		memset(&clientaddr, 0, sizeof(clientaddr));
		int addrlen = sizeof(clientaddr);
		recvfrom(ls, buf, BUF_MAX, 0, (sockaddr*)&clientaddr, &addrlen);

		printf("%s %d %s",buf, ntohs(clientaddr.sin_port), inet_ntoa(clientaddr.sin_addr));
		sendto(ls, buf, len, 0, (const sockaddr*)&clientaddr, sizeof(clientaddr));
	}


};