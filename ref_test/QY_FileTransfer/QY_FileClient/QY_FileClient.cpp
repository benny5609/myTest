
#include "QY_FileTransferImpl.h"
#include "ace/Thread.h"
#include "../QY_Common.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

static void* sendFile(void *arg);
string serverName = "tanqiyu";
int port = 2007;

int main(int argc, char *argv[])
{
	cout << "文件服务器: ";
	cin >> serverName;
	cout << "端口: ";
	cin >> port;
	
	QY_FileTransferImpl fileTransferObj;
	if(fileTransferObj.connectServer(serverName.c_str(), port) == false)
	{
		cout << "出错: 连接服务器失败\n";
	}
	else
	{
		cout << "连接服务器成功\n";
		cout << "输入要发送的文件名(多个文件名之间用空格分隔)\n:";

		vector<string> fileNameList;
		string fileName;
		string line;
		char ch;
		bool newNameFlag = false;
		
		ch = getchar();
		while(ch = getchar())
		{
			if(' ' == ch)
			{
				if(newNameFlag)
				{
					newNameFlag = false;
					if(fileName.empty() == false)
					{
						fileNameList.push_back(fileName);
						fileName = "";
					}
				}
			}
			else if('\n' == ch)
			{
				if(fileName.empty() == false)
				{
					fileNameList.push_back(fileName);
					fileName = "";
				}
				break;
			}
			else
			{
				newNameFlag = true;
				fileName += ch;
			}
		}

		for(vector<string>::iterator it = fileNameList.begin(); it != fileNameList.end(); it++)
		{
			if((*it).empty())
			{
				continue;
			}
			fileTransferObj.sendFile((*it).c_str());
		}		
		fileTransferObj.disconnect();
	}
	return NULL;
}

