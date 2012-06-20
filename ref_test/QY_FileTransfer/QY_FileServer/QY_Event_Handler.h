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

	//��Reactorע��READ_TASK�¼�����ʼ���peer_sock�ϵ���������
	virtual int open();

	//���տͻ��˵�����
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);	

	//���ͻ��˷�������
	virtual int handle_output(ACE_HANDLE = ACE_INVALID_HANDLE);	

	//ע���¼�����
	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);	

	ACE_SOCK_Stream& peer() { return this->peer_sock; }

	virtual ACE_HANDLE get_handle (void) const{ return this->peer_sock.get_handle(); }

private:
	//���ͻ��˷��ͷ�����Ϣ
	int feedback();	

private:
	//�����շ����ݵ���
	ACE_SOCK_Stream peer_sock;	

	//������Ҫ���͵���Ϣ����
	ACE_Message_Queue<ACE_NULL_SYNCH> output_queue;	

	//�����ļ�����
	QY_LocalFile localFile;	

	//��ʼ�����ļ���־
	bool recvFlag;	

	//���Կͻ��˵�����֡
	QY_ClientFrame receiveFrame;
	
	//���͵��ͻ��˵�����֡
	QY_ServerFrame sendFrame;	

	//�Ѿ�ע��WRITE_TASK��־
	bool writeTaskFlag; 
};

#endif
