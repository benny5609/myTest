#include "client_socket.h"
#include "server_thread.h"
#include "client_thread.h"
void ClientSocket::OnRecv(Packet &p)
{
	if (p.head_.op == op_jump)
	{
		int client_id = 0;
		memcpy(&client_id, &p.buf_[0], 4);
		g_clients.add_client(sock_,p.head_.guid);
		printf("client jump id %d", client_id);
	}
	int guid = g_clients.get_guid(sock_);
	if (guid == -1)
	{
		return;
	}
	p.head_.guid = guid;
	g_server.push(p);
}