
#ifndef MANAGE_LOGIN_HPP
#define MANAGE_LOGIN_HPP

#include <ace/Task.h>
#include <ace/Message_Queue_T.h>
#include "AppNotifyInterface.h"
#include "PacketStream.h"
#include "md5.h"

struct AccountInfo
{
	AccountInfo()
		: account_id(0)
	{}

	AccountInfo(const string & n_v, const string & p_v)
		: name(n_v)
		, password(p_v)
		, account_id(0)
	{}

	AccountInfo(const AccountInfo & rhs)
		: name(rhs.name)
		, password(rhs.password)
	{}

	string name;
	string password;
	int account_id;
	string session_code;
};

struct NetAddr 
{
	NetAddr()
		: port(0)
	{}

	string ip;
	int port;
};

struct LoginCfg 
{
	LoginCfg()
		: thread_no(1)
	{}

	int thread_no;
	NetAddr login_addr;
};

class ManageLogin : public AppNotifyInterface, public ACE_Task<ACE_NULL_SYNCH>
{
public:
	ManageLogin();
	~ManageLogin();
public:
	static ManageLogin * instance()
	{
		return Singleton<ManageLogin>::instance();
	}
public:
	/// init
	virtual int init(int argc, ACE_TCHAR * argv[]);

	/// fini, stop
	virtual int fini();

	/// wait for finish
	virtual int wait();
public:
	virtual int svc();

public:
	void handleAccount(AccountInfo * account_info);

protected:
	typedef ACE_Task<ACE_NULL_SYNCH> super;

	bool login(AccountInfo * account_info);

	PacketStream * makeLoginRequestPacket();

	PacketStream * makeLoginPacket(const string & name, const string & md5_code);

	void md5(const std::string& pw,const std::string& code, md5_byte_t * data);
private:
	LoginCfg m_login_cfg;

	ACE_Message_Queue_Ex<AccountInfo, ACE_MT_SYNCH> m_request_login_queue;

};
#endif