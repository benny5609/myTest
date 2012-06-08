#include "socket.h"
#include "logic_thread.h"
Socket::Socket(SOCKET s)
{
	recv_pos_ = 0;
	send_pos_ = 0;
	sock_ = s;
}
int Socket::update()
{
	int r = 0;
	if (send_pos_ !=0)
		r = send();

	if (r >=0 && send_pos_ == 0)
	{
		while(!send_queue_.empty())
		{
			Packet &msg = send_queue_.front();
			memcpy(send_buf_, &msg.head_, sizeof(msg.head_));
			send_pos_ += sizeof(msg.head_);
			memcpy(send_buf_+send_pos_, &msg.buf_[0], msg.buf_.size());
			send_pos_ += msg.buf_.size();
			send_queue_.pop();
			r = send();
			if (send_pos_ !=0 || r < 0)
				break;
		}
	}
	return r;
}
int Socket::recv() 
{
	int r = ::recv(sock_, &recv_buf_[recv_pos_], BUFF_SIZE - recv_pos_, 0);
	if (r < 0)
	{
	}else if(r == 0)
	{
	}else
	{
		recv_pos_ += r;
		while(recv_pos_ >= sizeof(Head))
		{
			Head* head = (Head*)recv_buf_;
			if (recv_pos_ >= sizeof(Head) + head->len)
			{
				Packet msg;
				msg.head_.op = head->op;
				msg.head_.guid = head->guid;
				msg.head_.len  = head->len;
				msg.buf_.resize(head->len);
				memcpy(&msg.buf_[0], recv_buf_ + sizeof(Head), head->len);
				recv_pos_ -= sizeof(Head);
				recv_pos_ -= head->len;
				memmove(recv_buf_,&recv_buf_[sizeof(Head)+head->len], recv_pos_);

				OnRecv(msg);
			}else
			{
				break;
			}
		}
	}

	return r;
}
int Socket::send()
{
	int r = ::send(sock_, send_buf_, send_pos_, 0);
	if (r > 0)
	{
		send_pos_ -= r;
		if (send_pos_ != 0)
		{
			memmove(send_buf_, &send_buf_[r], send_pos_);
		}
	}else if( r == 0)
	{
	}else
	{
		unsigned int err = GetLastError();
	}

	return r;
}

void Socket::close()
{
	delete this;
}
void Socket::push(Packet& p)
{
	send_queue_.push(p);
}