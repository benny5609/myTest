
#include "ace/OS.h"
#include "QY_FileTransferImpl.h"

#include <iostream>
using namespace std;


QY_FileTransferImpl::QY_FileTransferImpl()
{
}

QY_FileTransferImpl::~QY_FileTransferImpl()
{
}

bool QY_FileTransferImpl::connectServer(const char *serverName, int port)
{
	this->serverAddr.set(port, serverName);
	return this->conn.connect(peer_sock, serverAddr) == 0;
}

void QY_FileTransferImpl::disconnect()
{
	this->sendFrame.type = DISCONNECT_TYPE;
	this->sendAndReceive();
}

void QY_FileTransferImpl::sendFile(const char *fileName)
{
	if(ACE_OS::strlen(fileName) >= MAX_BUFFER_SIZE)
	{
		cout << "����: �ļ���̫��\n";
	}
	else if(QY_LocalFile::fileExist(fileName))
	{
		if(this->localFile.open(fileName))
		{
			if(this->sendFileName())
			{
				cout << "��ʼ�����ļ�: " << fileName << "\n";
				this->sendFileData();
			}
		}
		else
		{
			cout << "����: �޷����ļ�" << fileName << "\n";
		}
	}
	else
	{
		cout << "����: �ļ�" << fileName << "������\n";
	}
}

bool QY_FileTransferImpl::sendFileName()
{
	this->sendFrame.type = FILE_NAME_TYPE;
	ACE_OS::strcpy(this->sendFrame.buffer, this->localFile.getFileName());
	this->sendFrame.length = ACE_OS::strlen(this->localFile.getFileName());
	return this->sendAndReceive();
}

bool QY_FileTransferImpl::sendFileData()
{
	bool sendFlag = false;
	int readSize;
	while((readSize = this->localFile.read(this->sendFrame.buffer, MAX_BUFFER_SIZE)) > 0)
	{
		sendFlag = true;
		if(MAX_BUFFER_SIZE == readSize)
		{
			this->sendFrame.type = FILE_DATA_TYPE;			
		}
		
		if(readSize < MAX_BUFFER_SIZE)
		{
			this->sendFrame.type = FILE_END_TYPE;
		}
		this->sendFrame.length = readSize;
		if(this->sendAndReceive() == false)
		{
			return false;
		}		
	}

	//�����ļ���С��MAX_BUFFER_SIZE�������������
	if(sendFlag && 0 == readSize && this->sendFrame.type != FILE_END_TYPE)
	{
		this->sendFrame.type = FILE_END_TYPE;
		return this->sendAndReceive();
	}

	return true;
}

bool QY_FileTransferImpl::sendAndReceive()
{
	if(this->peer_sock.send_n(&(this->sendFrame), sizeof(QY_ClientFrame)) != sizeof(QY_ClientFrame))
	{
		cout << "����: �޷���������\n";
		return false;
	}

	if(DISCONNECT_TYPE == this->sendFrame.type)
	{
		return true;
	}
	
	if(this->peer_sock.recv_n(&(this->receiveFrame), sizeof(QY_ServerFrame)) != sizeof(QY_ServerFrame))
	{
		cout << "����: �޷����շ������˷���\n";
		return false;
	}

	switch(this->receiveFrame.type)
	{
	case OK_TYPE:
		return true;
	case FINISHED_TYPE:
		cout << "�ļ�" << this->localFile.getFileName() << "���ͳɹ�\n";
		return true;
	case FILE_EXIST_TYPE:
		{
			char c;
			cout << "�Ƿ񸲸��ļ�?[Y/N]: ";
			cin >> c;
			this->sendFrame.type = ('Y' == c || 'y' == c) ? FILE_OVERWRITE_TYPE : FILE_SKIP_TYPE;
			return this->sendAndReceive();
		}
	case ERROR_TYPE:
		cout << "����: �������˱������\n";
		return false;
	default:
		return false;
	}
}
