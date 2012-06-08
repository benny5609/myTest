#pragma comment(lib, "ws2_32.lib")
#include "client_thread.h"
#include "server_thread.h"
#include "logic_thread.h"
#include "accept_socket.h"

ClientThread g_clients;
LogicThread g_logic;
AcceptSocket g_accept;
ServerThread g_server;

int main()
{
	g_accept.init();
	g_server.init();
	//for (int i=0; i< SOCKET_THREADS; i++)
	{
		g_clients.init();
	}
	
	g_logic.run();

	return 0;
};