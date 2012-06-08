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
	laddr.sin_port = htons(9001);
	laddr.sin_addr.s_addr = ADDR_ANY;
	laddr.sin_family=AF_INET;
	bind(ls, (const sockaddr*)&laddr, sizeof(laddr));

	sockaddr_in saddr;
	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_port = htons(9000);
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_family=AF_INET;
	connect(ls, (const sockaddr*)&saddr, sizeof(saddr));

	__int64 value = BUF_MAX;
	int len = sizeof(value);
	setsockopt(ls, SOL_SOCKET,SO_SNDBUF, (char*)&value, len);
	getsockopt(ls, SOL_SOCKET, SO_SNDBUF, (char*)&value, &len);
	while(1)
	{
		char buf[BUF_MAX];
		int len = 0;
		memset(buf, 96, sizeof(buf));
		sendto(ls, buf, sizeof(buf), 0, (const sockaddr*)&saddr, sizeof(saddr));

		sockaddr_in serveraddr;
		memset(&serveraddr, 0, sizeof(serveraddr));
		int addrlen = sizeof(serveraddr);
		recvfrom(ls, buf, 10000, 0, (sockaddr*)&serveraddr, &addrlen);

		printf("%s %d %s",buf, ntohs(serveraddr.sin_port), inet_ntoa(serveraddr.sin_addr));

	}
};