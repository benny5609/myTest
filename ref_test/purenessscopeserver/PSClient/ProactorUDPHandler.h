#pragma once

#include "ace/ACE.h"
#include "ace/OS_main.h"   
#include "ace/Proactor.h"   
#include "ace/Asynch_IO.h"   
#include "ace/INET_Addr.h"   
#include "ace/SOCK_Dgram.h"   
#include "ace/Message_Block.h"   
#include "ace/Get_Opt.h"   

#include "define.h"

class CProactorUDPHandler : public ACE_Service_Handler
{
public:
	CProactorUDPHandler(void);
	~CProactorUDPHandler(void);

	int  OpenAddress(const ACE_INET_Addr& AddrRemote, ACE_Proactor* pProactor);
	void Close();

	virtual void handle_read_dgram(const ACE_Asynch_Read_Dgram::Result& result);
	bool SendMessage(const char* pMessage, int nLen, const ACE_INET_Addr& AddrRemote);


private:   
	ACE_SOCK_Dgram          m_skRemote;   
	ACE_Asynch_Read_Dgram   m_Read;
	ACE_Asynch_Write_Dgram  m_Write;
};
