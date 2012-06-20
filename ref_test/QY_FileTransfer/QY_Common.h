#ifndef QY_COMMON_H
#define QY_COMMON_H

//客户端数据帧的最大缓存大小
const int MAX_BUFFER_SIZE = 8*1024;

//服务器端数据帧的最大信息大小
const int MAX_INFO_SIZE = 64;

//客户端帧数据类型
enum QY_ClientFrameType
{
	FILE_NAME_TYPE,	//文件名称
	FILE_DATA_TYPE,	//文件数据
	FILE_END_TYPE,	//文件结尾
	FILE_ERROR_TYPE,//文件出错
	FILE_OVERWRITE_TYPE,	//覆盖文件
	FILE_SKIP_TYPE,	//跳过文件传送
	DISCONNECT_TYPE	//断开连接
};

//客户端数据帧
typedef struct
{
	QY_ClientFrameType type;
	int length;
	char buffer[MAX_BUFFER_SIZE];
}QY_ClientFrame;


//服务器端帧数据类型
enum QY_ServerFrameType
{
	INVALID_TYPE,	//保留的无效位，主要用来简化程序的控制逻辑
	OK_TYPE,		//表明服务器端正确接收到了客户端的数据
	FILE_EXIST_TYPE, //文件已经存在
	ERROR_TYPE,		//出错
	FINISHED_TYPE	//文件传输并保存成功
};

//服务器端数据帧
typedef struct
{
	QY_ServerFrameType type;
	int length;
	char info[MAX_INFO_SIZE];
}QY_ServerFrame;

#endif