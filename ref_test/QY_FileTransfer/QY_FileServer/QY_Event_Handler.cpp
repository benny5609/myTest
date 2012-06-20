
#include "QY_Event_Handler.h"
#include <iostream>
using namespace std;


int QY_Event_Handler::open()
{
	return this->reactor()->register_handler(this, ACE_Event_Handler::READ_MASK);
}

int QY_Event_Handler::handle_input(ACE_HANDLE)
{
	//接收数据
	int recvSize = this->peer_sock.recv_n(&(this->receiveFrame), sizeof(QY_ClientFrame));

	//接收数据错误
	if(recvSize != sizeof(QY_ClientFrame))	
	{
		return -1;
	}

	this->sendFrame.length = 0;
	this->sendFrame.type = INVALID_TYPE;

	//根据数据帧类型，采取相应的操作并准备好发送的数据帧
	switch(this->receiveFrame.type)
	{
	case FILE_NAME_TYPE:		//文件名	
		this->receiveFrame.buffer[this->receiveFrame.length] = '\0';

		//判断文件是否已经存在
		if(QY_LocalFile::fileExist(this->receiveFrame.buffer))
		{
			this->sendFrame.type = FILE_EXIST_TYPE;	
		}
		else if(this->localFile.open(this->receiveFrame.buffer))
		{
			this->sendFrame.type = OK_TYPE;
			this->recvFlag = true;
			cout << "开始接收文件" << this->localFile.getFileName() << "\n";
		}
		else
		{
			this->sendFrame.type = ERROR_TYPE;
		}
		break;
	case FILE_DATA_TYPE:	//文件数据
		//写文件
		if(this->localFile.write(this->receiveFrame.buffer, this->receiveFrame.length) < this->receiveFrame.length)
		{
			this->sendFrame.type = ERROR_TYPE;
		}
		else
		{
			this->sendFrame.type = OK_TYPE;
		}
		break;
	case FILE_END_TYPE:	//文件传输结束
		//写剩余数据
		if(this->receiveFrame.length > 0)
		{
			if(this->localFile.write(this->receiveFrame.buffer, this->receiveFrame.length) < this->receiveFrame.length)
			{
				this->sendFrame.type = ERROR_TYPE;
				break;
			}
		}
		this->localFile.close();
		this->sendFrame.type = FINISHED_TYPE;
		cout << "文件" << this->localFile.getFileName() <<"传送成功\n";
		this->recvFlag = false;
		break;
	case FILE_ERROR_TYPE:	//客户端出错
		//如果已经接收文件，则关闭并删除文件
		if(this->recvFlag)
		{
			this->localFile.close();
			QY_LocalFile::deleteFile(this->localFile.getFileName());
		}
		break;
	case FILE_OVERWRITE_TYPE:	//客户端确认覆盖文件
		this->receiveFrame.buffer[this->receiveFrame.length] = '\0';
		
		//打开文件并准备写数据
		if(this->localFile.open(this->receiveFrame.buffer))
		{
			this->sendFrame.type = OK_TYPE;
		}
		else
		{
			this->sendFrame.type = ERROR_TYPE;
		}		
		break;
	case FILE_SKIP_TYPE:	//客户端跳过该重名文件
		this->sendFrame.type = OK_TYPE;
		break;
	case DISCONNECT_TYPE:	//断开连接
		break;
	default:
		break;
	}

	//发送反馈信息
	if(feedback() == -1)
	{
		return -1;
	}

	//出错或者客户端断开则断开连接
	if(ERROR_TYPE == this->sendFrame.type 
		|| FILE_ERROR_TYPE == this->receiveFrame.type 
		|| DISCONNECT_TYPE == this->receiveFrame.type)
	{
		return -1;
	}

	return 0;
}

int QY_Event_Handler::handle_output(ACE_HANDLE)
{
	ACE_Message_Block *mb;
	ACE_Time_Value t(ACE_OS::gettimeofday());

	//从队列头部取消息块发送，循环直到队列为空
	while(this->output_queue.dequeue_head(mb, &t) == 0)
	{
		int sendSize = this->peer_sock.send_n(mb->rd_ptr(), mb->length());
		if(-1 == sendSize)
		{
			cout << "错误: 不能发送消息\n";
			break;
		}
		else 
		{
			mb->rd_ptr(sendSize);
		}

		//消息块没发送完，放入队列头部继续发送
		if(mb->length() > 0)
		{
			this->output_queue.enqueue_head(mb);
		}
		mb->release();
	}

	//若队列为空，则注销WRITE_TASK
	if(this->output_queue.is_empty())
	{
		this->writeTaskFlag = false;
		return -1;
	}
	else
	{
		return 0;
	}
}

int QY_Event_Handler::handle_close(ACE_HANDLE fd, ACE_Reactor_Mask mask)
{
	if (mask == ACE_Event_Handler::WRITE_MASK)
	{
		return 0;
	}

	ACE_INET_Addr clientAddr;
	if(this->peer_sock.get_remote_addr(clientAddr) == 0)
	{
		cout << "断开连接" << clientAddr.get_host_addr() << "\n";
	}
	
	mask = ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL;
	this->reactor()->remove_handler (this, mask);
	this->peer_sock.close();
	this->output_queue.flush ();
	delete this;
	return 0;
}

int QY_Event_Handler::feedback()
{
	if(this->sendFrame.type != INVALID_TYPE)
	{
		int sendSize = this->peer_sock.send_n(&(this->sendFrame), sizeof(QY_ServerFrame));

		if(sizeof(QY_ServerFrame) == sendSize)	//数据全部正确发送
		{
			return 0;
		}
		else if(-1 == sendSize)	//发送数据出错
		{
			cout << "错误: 不能发送反馈信息\n";
			return -1;
		}
		else if(sendSize > 0 && sendSize < sizeof(QY_ServerFrame)) //数据部分发送
		{
			ACE_Message_Block *mb;	//存放剩余的数据
			ACE_NEW_RETURN(mb, ACE_Message_Block((char*)&(this->sendFrame) + sendSize, sizeof(QY_ServerFrame) - sendSize), -1);
			ACE_Time_Value t(ACE_OS::gettimeofday());
			if(this->output_queue.enqueue_tail(mb, &t) == -1)
			{
				mb->release();
				return -1;
			}

			//注册WRITE_TASK
			if(!(this->writeTaskFlag))
			{
				return this->reactor()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
			}
		}
	}

	return 0;
}