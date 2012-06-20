#ifndef QY_ACCEPTOR_H
#define QY_ACCEPTOR_H

#include "ace/Reactor.h"
#include "ace/Event_Handler.h"
#include "ace/SOCK_Acceptor.h"

//�ͻ��������¼�����
class QY_Acceptor : public ACE_Event_Handler
{
public:
	QY_Acceptor(ACE_Reactor *r = ACE_Reactor::instance()) : ACE_Event_Handler(r) {}
	virtual ~QY_Acceptor() { this->handle_close();}

	//�򿪶˿�ע�ᣬ����ʼ������Ĭ�϶˿ں���2007
	virtual int open(int = 2007);

	//������������
	virtual int handle_input(ACE_HANDLE = ACE_INVALID_HANDLE);

	virtual int handle_close(ACE_HANDLE = ACE_INVALID_HANDLE, ACE_Reactor_Mask = 0);

	virtual ACE_HANDLE get_handle() const { return this->sock_acceptor.get_handle(); }

private:
	ACE_SOCK_Acceptor sock_acceptor;
};

#endif