#ifndef QY_LOCAL_FILE_H
#define QY_LOCAL_FILE_H

#include "ace/FILE_Addr.h"
#include "ace/FILE_IO.h"
#include "ace/FILE_Connector.h"

#include <string>
using namespace std;


class QY_LocalFile
{
public:
	//������fileName��Ϊ�գ��򴴽�QY_LocalFile�����ļ�
	QY_LocalFile(const char *fileName = NULL);	
	virtual ~QY_LocalFile();

	//�ж��ļ��Ƿ����
	static bool fileExist(const char *fileName);	

	//�����ļ�
	static bool createFile(const char *fileName);

	//ɾ���ļ�
	static bool deleteFile(const char *fileName);	

	//���ļ�
	bool open(const char *fileName);

	//�ر��ļ�
	void close();

	//���ļ����������
	int write(char *data, int length);

	//���ļ��ж�ȡ����
	int read(char *data, int length);

	//��ȡ�ļ���·����
	const char* getPathName() const { return this->fileAddr.get_path_name(); }

	//��ȡ�ļ���
	const char* getFileName() const { return this->pureFileName.c_str();}

private:
	//���治��·���Ĵ��ļ���
	void cacheFileName(const char *fileName);

private:
	//�ļ���ַ����
	ACE_FILE_Addr fileAddr;	

	//�ļ������������
	ACE_FILE_IO fileIO;

	//����·�����ļ���
	string pureFileName;
};

#endif