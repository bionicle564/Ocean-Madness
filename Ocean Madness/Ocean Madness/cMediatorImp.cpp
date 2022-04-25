#include "cMediatorImp.h"

cMediatorImp::~cMediatorImp()
{
}

bool cMediatorImp::SetReciever(iMediator* pTheReciever)
{
	return false;
}

bool cMediatorImp::RecieveMessage(sMessage theMessage)
{
	return false;
}

bool cMediatorImp::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}
