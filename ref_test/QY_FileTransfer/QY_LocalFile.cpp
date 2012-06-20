
#include "QY_LocalFile.h"

QY_LocalFile::QY_LocalFile(const char *fileName)
{
	if(fileName != NULL)
	{
		this->fileAddr.set(fileName);
		this->open(fileName);
	}
}

QY_LocalFile::~QY_LocalFile()
{
	this->close();
}

//判断文件是否存在
bool QY_LocalFile::fileExist(const char *fileName)
{
	if(NULL == fileName)
	{
		return false;
	}

	ACE_FILE_Connector conn;
	ACE_FILE_IO dfile;
	return conn.connect(dfile, ACE_FILE_Addr(fileName),
						0, ACE_Addr::sap_any, 0, O_RDWR,
						ACE_DEFAULT_FILE_PERMS
					   ) == 0;
}

//创建文件
bool QY_LocalFile::createFile(const char *fileName)
{
	if(NULL == fileName)
	{
		return false;
	}

	ACE_FILE_Connector conn;
	ACE_FILE_IO dfile;
	return conn.connect(dfile, ACE_FILE_Addr(fileName)) == 0;
}

//删除文件
bool QY_LocalFile::deleteFile(const char *fileName)
{
	if(NULL == fileName)
	{
		return false;
	}

	ACE_FILE_Connector conn;
	ACE_FILE_IO dfile;
	if(conn.connect(dfile, ACE_FILE_Addr(fileName)) == -1)
	{
		return false;
	}

	return dfile.remove() == 0;
}

//打开文件
bool QY_LocalFile::open(const char *fileName)
{
	if(NULL == fileName)
	{
		return false;
	}

	ACE_FILE_Connector conn;
	this->fileAddr.set(fileName);
	this->cacheFileName(fileName);
	return conn.connect(this->fileIO, this->fileAddr) == 0;
}

//关闭文件
void QY_LocalFile::close()
{
	this->fileIO.close();
}

//往文件中添加数据
int QY_LocalFile::write(char *data, int length)
{
	return this->fileIO.send_n(data, length);
}

//从文件中读取数据
int QY_LocalFile::read(char *data, int length)
{
	return this->fileIO.recv(data, length);
}

//缓存不含路径的纯文件名
void QY_LocalFile::cacheFileName(const char *fileName)
{
	int len = strlen(fileName);
	this->pureFileName = "";
	for(int i = len - 1; i >= 0; i--)
	{
		if('\\' == fileName[i])
		{
			break;
		}
		else
		{
			this->pureFileName.insert(0, fileName[i]);
		}
	}
}

