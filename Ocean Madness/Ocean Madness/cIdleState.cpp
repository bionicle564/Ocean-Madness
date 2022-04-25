#include "cIdleState.h"
#include "cAiMesh.h"

void cIdleState::Enter(cAiMesh* body)
{
	body->PlayAnimation(2);
	body->controller->loop = true;
}

void cIdleState::Update(cAiMesh* body, float deltaTime)
{
}

void cIdleState::Toggle(cAiMesh* body)
{
}

void cIdleState::Exit(cAiMesh* body)
{
}
