#include "cBlendState.h"
#include "cAiMesh.h"

cBlendState::cBlendState(cAiMesh* body, int animationTo, cMeshState* nextState)
{
	this->blendAnimation = body->BlendAnimation(animationTo);
	this->nextState = nextState;
}

cBlendState::~cBlendState()
{
	delete blendAnimation;
}

void cBlendState::Enter(cAiMesh* body)
{
	body->controller->loop = false;
}

void cBlendState::Update(cAiMesh* body, float deltaTime)
{
	if (body->controller->isDone)
	{
		this->Toggle(body);
	}
}

void cBlendState::Toggle(cAiMesh* body)
{
	body->SetState(this->nextState);
}

void cBlendState::Exit(cAiMesh* body)
{
}
