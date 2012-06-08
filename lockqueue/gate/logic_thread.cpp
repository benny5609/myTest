#include "logic_thread.h"
#include "client_thread.h"
int i=0;
int li=0;
void LogicThread::run()
{
	while(1)
	{
		WaitForSingleObject(recv_queue_.get_event(), INFINITE);
		Packet p;
		while(recv_queue_.pop(p))
		{
			//if (p.op == op_enter)
			//{
			//	players_.insert(p.guid);
			//	printf("player enter ,value=%d size = %d", p.guid, players_.size());
			//}else if (p.op== op_leave)
			//{
			//	players_.erase(p.guid);
			//	printf("player leave ,value=%d size = %d", p.guid, players_.size());
			//}else
			//{
			//	int a;
			//	memcpy(&a,&p.buf[0],sizeof(a));
			//	//printf("recv op=%d,guid=%d,value=%d", p.op,p.guid, a);
			//	i++;
			//	
			//	static int aa = GetTickCount();
			//	if (GetTickCount() - aa > 10000)
			//	{
			//		printf("packets %d %d",SOCKET_THREADS, i-li);
			//		li = i;
			//		aa = GetTickCount();
			//	}
			//	g_clients[p.guid%SOCKET_THREADS].push(p);
			//}
		}
		Sleep(1);
	}
}