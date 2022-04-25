#pragma once
#include "iMessage.h"
#include "cAiMesh.h"

class cBoat : public iMessage
{
public:

	glm::vec3 worldPosition;



	std::vector<cAiMesh> projectileMeshes;

	cAiMesh boat;
	cAiMesh ocean;
	cAiMesh starboardCannon;
	cAiMesh portCannon;
	cAiMesh ammoPile;
	cAiMesh wheel;

	cBoat(iMediator* mediator, GLuint* programs);

	// This sets the mediator so that the objects can talk to it
	virtual bool SetReciever(iMediator* pTheReciever);

	// Asynchronous (don't return)
	virtual bool RecieveMessage(sMessage theMessage);

	// Synchronous (do return with something)
	virtual bool RecieveMessage(sMessage theMessage, sMessage& messageReply);

	void Update(float deltaTime);

	void Render(GLuint* programs, glm::vec3 eye, glm::vec3 at);

private:
	glm::vec3 lastStep;
	glm::vec3 currentStep;


	iMediator* mediator;
};