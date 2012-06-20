#ifndef QY_FILE_TRANSFER_IMPL_H
#define QY_FILE_TRANSFER_IMPL_H

#include "../QY_Common.h"
#include "../QY_LocalFile.h"

#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"


class QY_FileTransferImpl
{
public:
	QY_FileTransferImpl();
	virtual ~QY_FileTransferImpl();

	//连接服务器
	bool connectServer(const char *serverName, int port);

	//发送文件
	void sendFile(const char *fileName);

	//断开连接
	void disconnect();

private:
	//发送文件名
	bool sendFileName();

	//发送文件数据
	bool sendFileData();

	//发送客户端数据帧并接收服务器端反馈
	bool sendAndReceive();

private:
	ACE_INET_Addr serverAddr;
	ACE_SOCK_Connector conn;
	ACE_SOCK_Stream peer_sock;

	QY_LocalFile localFile;
	QY_ClientFrame sendFrame;
	QY_ServerFrame receiveFrame;
};

#endif