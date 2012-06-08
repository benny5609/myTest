#include "server_socket.h"
#include "client_thread.h"
#include "server_thread.h"
void ServerSocket::OnRecv(Packet &p)
{
	if (p.head_.op == op_addserver)
	{
		int server_id = 0;
		memcpy(&server_id, &p.buf_[0], 4);
		g_server.add_server(server_id, sock_);
		printf("add server id %d", server_id);
		return;
	}
	if (p.head_.op == op_jump_success)
	{
		g_server.clienttoserver_.erase(p.head_.guid);
	}
		// 路由消息
	g_clients.push(p);
	
}