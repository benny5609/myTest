
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
	cout << "�ļ�������: ";
	cin >> serverName;
	cout << "�˿�: ";
	cin >> port;
	
	QY_FileTransferImpl fileTransferObj;
	if(fileTransferObj.connectServer(serverName.c_str(), port) == false)
	{
		cout << "����: ���ӷ�����ʧ��\n";
	}
	else
	{
		cout << "���ӷ������ɹ�\n";
		cout << "����Ҫ���͵��ļ���(����ļ���֮���ÿո�ָ�)\n:";

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

