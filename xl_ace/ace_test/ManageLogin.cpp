
#include <iostream>
#include <ace/SOCK_Stream.h>
#include <ace/Sock_Connector.h>
#include <ace/OS.h>
#include "opcodes.hpp"
#include "ManageLogin.h"
#include "login.pb.h"
#include "md5.h"

bool sendPacket(ACE_SOCK_Stream & sock_stream, PacketStream * packet_stream)
{
	ACE_Time_Value time_out(60);
	size_t ss = sock_stream.send_n(packet_stream->stream(), packet_stream->stream_size(), &time_out);
	if (-1 == ss)
	{
		TRACELogp(LM_ERROR, ACE_TEXT("failed to send data to login, last error is <%d>\n"), ACE_OS::last_error());
		return false;
	}
	else
	{
		return true;
	}
}

PacketStream * recvPacket(ACE_SOCK_Stream & sock_stream)
{
	ACE_Time_Value time_out(60);
	auto_ptr<PacketStream> ps(new PacketStream());
	if (-1 == sock_stream.recv_n(ps->re_head(), ps->head_size(), &time_out))
	{
		TRACELogp(LM_ERROR, ACE_TEXT("Failed to recv data from login, last error is <%d>\n"), ACE_OS::last_error());
		return NULL;
	}

	if (-1 == sock_stream.recv_n(ps->re_body(), ps->body_size()))
	{
		TRACELogp(LM_ERROR, ACE_TEXT("Failed to recv data from login, last error is <%d>\n"), ACE_OS::last_error());
		return NULL;
	}

	return ps.release();
}

ManageLogin::ManageLogin()
{
	m_login_cfg.login_addr.ip = "172.16.12.140";
	m_login_cfg.login_addr.port = 4096;
}

ManageLogin::~ManageLogin()
{

}

int ManageLogin::init(int argc, ACE_TCHAR * argv[])
{
	if (this->activate(THR_JOINABLE) == -1)
	{
		return -1;
	}
	return 0;
}

int ManageLogin::fini()
{
	m_stop = true;
	return 0;
}

int ManageLogin::wait()
{
	m_wait = true;
	return super::wait();
}

int ManageLogin::svc()
{
	while (!m_stop)
	{
		if (m_request_login_queue.is_empty())
		{
			ACE_OS::sleep(3);
			continue;
		}

		ACE_Time_Value time_out(3, 0);
		AccountInfo * account_info = NULL;
		while (m_request_login_queue.dequeue_head(account_info, &time_out) >=0)
		{
			if (login(account_info))
			{
				std::cout << "success to login" << std::endl;
			}
			else
			{
			}
		}
	}
	return 0;
}

void ManageLogin::handleAccount(AccountInfo * account_info)
{
	m_request_login_queue.enqueue_tail(account_info);
}

bool ManageLogin::login(AccountInfo * account_info)
{
	ACE_INET_Addr addr(m_login_cfg.login_addr.port, m_login_cfg.login_addr.ip.c_str());
	ACE_SOCK_Stream sock_stream;
	ACE_SOCK_Connector sock_connector;
	if (sock_connector.connect(sock_stream, addr) == -1)
	{
		return false;
	}

	auto_ptr<PacketStream> ps(makeLoginRequestPacket());
	if (!sendPacket(sock_stream, ps.get()))
	{
		return false;
	}
	
	ps.reset(recvPacket(sock_stream));
	if (!ps.get())
	{
		return false;
	}

	LoginServer::smsg_loginRequest_reply replay_msg;
	if (!ps->parse_protocol(replay_msg))
	{
		return false;
	}
	
	md5_byte_t data[16] = {0};
	md5(account_info->password, replay_msg.code(), data);
	std::string md5_code((char*)data,16);

	ps.reset(makeLoginPacket(account_info->name, md5_code));
	if (!ps.get())
	{
		return false;
	}

	if (!sendPacket(sock_stream, ps.get()))
	{
		return false;
	}
	
	ps.reset(recvPacket(sock_stream));
	if (!ps.get())
	{
		return false;
	}


	if (ps->opcode() == SMSG_AUTH_SUCCESS)
	{
		LoginServer::smsg_Auth_Success auth_msg;
		if (!ps->parse_protocol(auth_msg))
		{
			return false;
		}
		
		account_info->account_id = auth_msg.accountid();
		account_info->session_code = auth_msg.sessionid();
		return true;
	}
	else
	{
		LoginServer::smsg_Result result;
		if (!ps->parse_protocol(result))
		{
			return false;
		}
		TRACELogp(LM_ERROR, ACE_TEXT("failed to recv auth packet from login, opcode is <%d>, return error is <%d>\n"), ps->opcode(), result.rescode());
		return false;
	}
}

PacketStream * ManageLogin::makeLoginRequestPacket()
{
	LoginServer::cmsg_login_request login_req;
	login_req.set_code("D31D01904C044669AE78904FCDD966AE");
	return new PacketStream (CMSG_LOGIN_ACCOUNT_Req, 1 , login_req.SerializeAsString());
}

PacketStream * ManageLogin::makeLoginPacket(const string & name, const string & md5_code)
{
	LoginServer::cmsg_LoginAccount login;
	login.set_username(name);
	login.set_password(md5_code);
	return new PacketStream(CMSG_LOGIN_ACCOUNT, 1 , login.SerializeAsString());
}

void ManageLogin::md5(const std::string& pw,const std::string& code, md5_byte_t * data)
{
	md5_state_t state;
	md5_init(&state);
	md5_append(&state,(const md5_byte_t*)pw.c_str(),pw.size());
	md5_append(&state,(const md5_byte_t*)code.c_str(),code.size());
	md5_finish(&state,data);
}