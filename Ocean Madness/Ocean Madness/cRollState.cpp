#include "cRollState.h"
#include "cAiMesh.h"
#include "cRunState.h"

void cRollState::Enter(cAiMesh* body)
{
	body->PlayAnimation(7);
	body->controller->loop = false;
}

void cRollState::Update(cAiMesh* body, float deltaTime)
{
	if (body->controller->isDone)
	{
		this->Toggle(body);
	}
}

void cRollState::Toggle(cAiMesh* body)
{
	if (body->controller->isDone) //only change if we are ready
	{
		body->SetState(new cRunState());
	}
}

void cRollState::Exit(cAiMesh* body)
{
}
