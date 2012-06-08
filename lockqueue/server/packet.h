#ifndef _PACKET_H_
#define _PACKET_H_
#include <vector>
enum Opcode
{
	op_none,
	op_new,
	op_delete,
	op_jump,
	op_jump_success,
	op_addserver,
	op_move,
};
struct Head 
{
	int op;
	int len;
	int guid;
};
class Packet 
{
public:
	Packet()
	{
		head_.op = op_none;
		head_.len= 0;
		head_.guid=0;
	}
	Packet(Opcode op, unsigned short len, int guid, const char* buf)
	{
		head_.op = op;
		head_.len=len;
		head_.guid=guid;
		buf_.reserve(len);
		memcpy(&(buf_[0]), buf, len);
	}
	Packet(const Packet& buf)
	{
		head_.op = buf.head_.op;
		head_.len= buf.head_.len;
		head_.guid=buf.head_.guid;
		buf_ = buf.buf_;
	}
	Packet &operator=(const Packet& buf)
	{
		if (this != &buf)
		{
			head_.op = buf.head_.op;
			head_.len= buf.head_.len;
			head_.guid=buf.head_.guid;
			buf_ = buf.buf_;
		}
		return *this;
	}
	~Packet()
	{

	}

	Head head_;
	std::vector<char> buf_;
};

#endif