#pragma once
#include "iMediator.h"


class cMediatorImp : public iMediator
{
public:

	cMediatorImp() {}
	~cMediatorImp();

	// ****************************************
	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);
	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);
	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);
};