#pragma once
#include <string>
#include <vector>	
#include <glm/vec4.hpp>

struct sMessage
{
public:
	sMessage() {}
	std::string command;
	std::vector< std::string > vec_sData;
	std::vector< glm::vec4 > vec_fData;

	std::vector< void* > vec_pVoidPointers;
};

class iMediator;

class iMessage
{
public:
	// Make a virtual destructor for anything with virtual
	virtual ~iMessage() {}

	// This sets the mediator so that the objects can talk to it
	virtual bool SetReciever(iMediator* pTheReciever) = 0;

	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage) = 0;

	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply) = 0;

};