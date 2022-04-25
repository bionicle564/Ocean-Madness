#pragma once

#include "iMediator.h"

class cMediatorImp;

class cMediator : public iMediator
{
private:
	cMediatorImp* m_Imp;

public:

	cMediator();
	virtual ~cMediator();

	
	// From the iMessage interface
	virtual bool SetReciever(iMediator* pTheReciever);

	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);

	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);

	
};