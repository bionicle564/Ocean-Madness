#pragma once

#include "cMeshState.h"
#include "cAnimation.h"

class cBlendState : public cMeshState
{
private:
	cAnimation* blendAnimation = nullptr;
	cMeshState* nextState;
public:

	cBlendState(cAiMesh* body, int animationTo, cMeshState* nextState);
	~cBlendState();

	virtual void Enter(cAiMesh* body);
	virtual void Update(cAiMesh* body, float deltaTime);
	virtual void Toggle(cAiMesh* body);
	virtual void Exit(cAiMesh* body);
};