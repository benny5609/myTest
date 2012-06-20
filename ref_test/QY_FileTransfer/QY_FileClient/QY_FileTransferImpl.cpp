
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
		cout << "错误: 文件名太长\n";
	}
	else if(QY_LocalFile::fileExist(fileName))
	{
		if(this->localFile.open(fileName))
		{
			if(this->sendFileName())
			{
				cout << "开始发送文件: " << fileName << "\n";
				this->sendFileData();
			}
		}
		else
		{
			cout << "错误: 无法打开文件" << fileName << "\n";
		}
	}
	else
	{
		cout << "错误: 文件" << fileName << "不存在\n";
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

	//处理文件大小是MAX_BUFFER_SIZE的整数倍的情况
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
		cout << "出错: 无法发送数据\n";
		return false;
	}

	if(DISCONNECT_TYPE == this->sendFrame.type)
	{
		return true;
	}
	
	if(this->peer_sock.recv_n(&(this->receiveFrame), sizeof(QY_ServerFrame)) != sizeof(QY_ServerFrame))
	{
		cout << "出错: 无法接收服务器端反馈\n";
		return false;
	}

	switch(this->receiveFrame.type)
	{
	case OK_TYPE:
		return true;
	case FINISHED_TYPE:
		cout << "文件" << this->localFile.getFileName() << "发送成功\n";
		return true;
	case FILE_EXIST_TYPE:
		{
			char c;
			cout << "是否覆盖文件?[Y/N]: ";
			cin >> c;
			this->sendFrame.type = ('Y' == c || 'y' == c) ? FILE_OVERWRITE_TYPE : FILE_SKIP_TYPE;
			return this->sendAndReceive();
		}
	case ERROR_TYPE:
		cout << "出错: 服务器端报告错误\n";
		return false;
	default:
		return false;
	}
}
