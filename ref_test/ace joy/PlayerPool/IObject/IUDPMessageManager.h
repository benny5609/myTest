#ifndef _IUDPMESSAGEMANAGER_H
#define _IUDPMESSAGEMANAGER_H

#include "ClientCommand.h"
#include "IMessage.h"

class IUDPMessageManager
{
public:
	virtual ~IUDPMessageManager() {};

	virtual bool DoMessage(IMessage* pMessage, uint32& u4Len, uint16& u2CommandID)    = 0;
	virtual void Close()                                                              = 0;

	virtual bool AddClientCommand(uint16 u2CommandID, CClientCommand* pClientCommand) = 0;
	virtual bool DelClientCommand(uint16 u2CommandID)                                 = 0;
};

#endif
