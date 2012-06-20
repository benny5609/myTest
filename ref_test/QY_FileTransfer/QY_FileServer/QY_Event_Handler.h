#ifndef QY_EVENT_HANDLER_H
#define QY_EVENT_HANDLER_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Synch.h"
#include "ace/Message_Block.h"
#include "ace/Message_Queue.h"

#include "../QY_LocalFile.h"
#include "../QY_Common.h"


class QY_Event_Handler : public ACE_Event_Handler
{
public:
	QY_Event_Handler(ACE_Reactor *r = ACE_Reactor::instance()) 
		: ACE_Event_Handler(r), recvFlag(false), writeTaskFlag(false)
	{}
	virtual ~QY_Event_Handler() {}

	//向Reactor注册READ_TASK事件，开始监测peer_sock上的数据输入
	virtual int open();

	//接收客户端的数据
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);	

	//往客户端发送数据
	virtual int handle_output(ACE_HANDLE = ACE_INVALID_HANDLE);	

	//注销事件处理
	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);	

	ACE_SOCK_Stream& peer() { return this->peer_sock; }

	virtual ACE_HANDLE get_handle (void) const{ return this->peer_sock.get_handle(); }

private:
	//往客户端发送反馈信息
	int feedback();	

private:
	//用来收发数据的流
	ACE_SOCK_Stream peer_sock;	

	//缓存需要发送的消息队列
	ACE_Message_Queue<ACE_NULL_SYNCH> output_queue;	

	//本地文件对象
	QY_LocalFile localFile;	

	//开始接收文件标志
	bool recvFlag;	

	//来自客户端的数据帧
	QY_ClientFrame receiveFrame;
	
	//发送到客户端的数据帧
	QY_ServerFrame sendFrame;	

	//已经注册WRITE_TASK标志
	bool writeTaskFlag; 
};

#endif
