#include "cMediator.h"

#include "cMediatorImp.h"

cMediator::cMediator()
{
	m_Imp = new cMediatorImp();
}

cMediator::~cMediator()
{
	delete m_Imp;
}

bool cMediator::SetReciever(iMediator* pTheReciever)
{
	this->m_Imp->SetReciever(pTheReciever);
	return true;
}

bool cMediator::RecieveMessage(sMessage theMessage)
{
	this->m_Imp->RecieveMessage(theMessage);
	return true;
}

bool cMediator::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return this->m_Imp->RecieveMessage(theMessage, messageReply);
}
