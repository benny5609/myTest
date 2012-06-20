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

	//���ӷ�����
	bool connectServer(const char *serverName, int port);

	//�����ļ�
	void sendFile(const char *fileName);

	//�Ͽ�����
	void disconnect();

private:
	//�����ļ���
	bool sendFileName();

	//�����ļ�����
	bool sendFileData();

	//���Ϳͻ�������֡�����շ������˷���
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