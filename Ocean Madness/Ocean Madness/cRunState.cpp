#include "cRunState.h"
#include "cAiMesh.h"
#include "cRollState.h"

void cRunState::Enter(cAiMesh* body)
{
	body->PlayAnimation(9);
	body->controller->loop = true;
}

void cRunState::Update(cAiMesh* body, float deltaTime)
{
}

void cRunState::Toggle(cAiMesh* body)
{
	//body->SetState(new cRollState());
}

void cRunState::Exit(cAiMesh* body)
{
}
