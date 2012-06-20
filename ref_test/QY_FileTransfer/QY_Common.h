#ifndef QY_COMMON_H
#define QY_COMMON_H

//�ͻ�������֡����󻺴��С
const int MAX_BUFFER_SIZE = 8*1024;

//������������֡�������Ϣ��С
const int MAX_INFO_SIZE = 64;

//�ͻ���֡��������
enum QY_ClientFrameType
{
	FILE_NAME_TYPE,	//�ļ�����
	FILE_DATA_TYPE,	//�ļ�����
	FILE_END_TYPE,	//�ļ���β
	FILE_ERROR_TYPE,//�ļ�����
	FILE_OVERWRITE_TYPE,	//�����ļ�
	FILE_SKIP_TYPE,	//�����ļ�����
	DISCONNECT_TYPE	//�Ͽ�����
};

//�ͻ�������֡
typedef struct
{
	QY_ClientFrameType type;
	int length;
	char buffer[MAX_BUFFER_SIZE];
}QY_ClientFrame;


//��������֡��������
enum QY_ServerFrameType
{
	INVALID_TYPE,	//��������Чλ����Ҫ�����򻯳���Ŀ����߼�
	OK_TYPE,		//��������������ȷ���յ��˿ͻ��˵�����
	FILE_EXIST_TYPE, //�ļ��Ѿ�����
	ERROR_TYPE,		//����
	FINISHED_TYPE	//�ļ����䲢����ɹ�
};

//������������֡
typedef struct
{
	QY_ServerFrameType type;
	int length;
	char info[MAX_INFO_SIZE];
}QY_ServerFrame;

#endif