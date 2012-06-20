
#include "QY_Event_Handler.h"
#include <iostream>
using namespace std;


int QY_Event_Handler::open()
{
	return this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
}

int QY_Event_Handler::handle_input(ACE_HANDLE)
{
	//��������
	int recvSize = this->peer_sock.recv_n(&(this->receiveFrame), sizeof(QY_ClientFrame));

	//�������ݴ���
	if(recvSize != sizeof(QY_ClientFrame))	
	{
		return -1;
	}

	this->sendFrame.length = 0;
	this->sendFrame.type = INVALID_TYPE;

	//��������֡���ͣ���ȡ��Ӧ�Ĳ�����׼���÷��͵�����֡
	switch(this->receiveFrame.type)
	{
	case FILE_NAME_TYPE:		//�ļ���	
		this->receiveFrame.buffer[this->receiveFrame.length] = '\0';

		//�ж��ļ��Ƿ��Ѿ�����
		if(QY_LocalFile::fileExist(this->receiveFrame.buffer))
		{
			this->sendFrame.type = FILE_EXIST_TYPE;	
		}
		else if(this->localFile.open(this->receiveFrame.buffer))
		{
			this->sendFrame.type = OK_TYPE;
			this->recvFlag = true;
			cout << "��ʼ�����ļ�" << this->localFile.getFileName() << "\n";
		}
		else
		{
			this->sendFrame.type = ERROR_TYPE;
		}
		break;
	case FILE_DATA_TYPE:	//�ļ�����
		//д�ļ�
		if(this->localFile.write(this->receiveFrame.buffer, this->receiveFrame.length) < this->receiveFrame.length)
		{
			this->sendFrame.type = ERROR_TYPE;
		}
		else
		{
			this->sendFrame.type = OK_TYPE;
		}
		break;
	case FILE_END_TYPE:	//�ļ��������
		//дʣ������
		if(this->receiveFrame.length > 0)
		{
			if(this->localFile.write(this->receiveFrame.buffer, this->receiveFrame.length) < this->receiveFrame.length)
			{
				this->sendFrame.type = ERROR_TYPE;
				break;
			}
		}
		this->localFile.close();
		this->sendFrame.type = FINISHED_TYPE;
		cout << "�ļ�" << this->localFile.getFileName() <<"���ͳɹ�\n";
		this->recvFlag = false;
		break;
	case FILE_ERROR_TYPE:	//�ͻ��˳���
		//����Ѿ������ļ�����رղ�ɾ���ļ�
		if(this->recvFlag)
		{
			this->localFile.close();
			QY_LocalFile::deleteFile(this->localFile.getFileName());
		}
		break;
	case FILE_OVERWRITE_TYPE:	//�ͻ���ȷ�ϸ����ļ�
		this->receiveFrame.buffer[this->receiveFrame.length] = '\0';
		
		//���ļ���׼��д����
		if(this->localFile.open(this->receiveFrame.buffer))
		{
			this->sendFrame.type = OK_TYPE;
		}
		else
		{
			this->sendFrame.type = ERROR_TYPE;
		}		
		break;
	case FILE_SKIP_TYPE:	//�ͻ��������������ļ�
		this->sendFrame.type = OK_TYPE;
		break;
	case DISCONNECT_TYPE:	//�Ͽ�����
		break;
	default:
		break;
	}

	//���ͷ�����Ϣ
	if(feedback() == -1)
	{
		return -1;
	}

	//������߿ͻ��˶Ͽ���Ͽ�����
	if(ERROR_TYPE == this->sendFrame.type 
		|| FILE_ERROR_TYPE == this->receiveFrame.type 
		|| DISCONNECT_TYPE == this->receiveFrame.type)
	{
		return -1;
	}

	return 0;
}

int QY_Event_Handler::handle_output(ACE_HANDLE)
{
	ACE_Message_Block *mb;
	ACE_Time_Value t(ACE_OS::gettimeofday());

	//�Ӷ���ͷ��ȡ��Ϣ�鷢�ͣ�ѭ��ֱ������Ϊ��
	while(this->output_queue.dequeue_head(mb, &t) == 0)
	{
		int sendSize = this->peer_sock.send_n(mb->rd_ptr(), mb->length());
		if(-1 == sendSize)
		{
			cout << "����: ���ܷ�����Ϣ\n";
			break;
		}
		else 
		{
			mb->rd_ptr(sendSize);
		}

		//��Ϣ��û�����꣬�������ͷ����������
		if(mb->length() > 0)
		{
			this->output_queue.enqueue_head(mb);
		}
		mb->release();
	}

	//������Ϊ�գ���ע��WRITE_TASK
	if(this->output_queue.is_empty())
	{
		this->writeTaskFlag = false;
		return -1;
	}
	else
	{
		return 0;
	}
}

int QY_Event_Handler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask mask)
{
	if (mask == ACE_Event_Handler::WRITE_MASK)
	{
		return 0;
	}

	ACE_INET_Addr clientAddr;
	if(this->peer_sock.get_remote_addr(clientAddr) == 0)
	{
		cout << "�Ͽ�����" << clientAddr.get_host_addr() << "\n";
	}
	
	mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
	this->reactor()->remove_handler (this, mask);
	this->peer_sock.close();
	this->output_queue.flush ();
	delete this;
	return 0;
}

int QY_Event_Handler::feedback()
{
	if(this->sendFrame.type != INVALID_TYPE)
	{
		int sendSize = this->peer_sock.send_n(&(this->sendFrame), sizeof(QY_ServerFrame));

		if(sizeof(QY_ServerFrame) == sendSize)	//����ȫ����ȷ����
		{
			return 0;
		}
		else if(-1 == sendSize)	//�������ݳ���
		{
			cout << "����: ���ܷ��ͷ�����Ϣ\n";
			return -1;
		}
		else if(sendSize > 0 && sendSize < sizeof(QY_ServerFrame)) //���ݲ��ַ���
		{
			ACE_Message_Block *mb;	//���ʣ�������
			ACE_NEW_RETURN(mb, ACE_Message_Block((char*)&(this->sendFrame) + sendSize, sizeof(QY_ServerFrame) - sendSize), -1);
			ACE_Time_Value t(ACE_OS::gettimeofday());
			if(this->output_queue.enqueue_tail(mb, &t) == -1)
			{
				mb->release();
				return -1;
			}

			//ע��WRITE_TASK
			if(!(this->writeTaskFlag))
			{
				return this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
			}
		}
	}

	return 0;
}