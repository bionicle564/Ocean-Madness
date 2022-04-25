#pragma once

#include "cMeshState.h"

class cRollState : public cMeshState
{
public:

	virtual void Enter(cAiMesh* body);
	virtual void Update(cAiMesh* body, float deltaTime);
	virtual void Toggle(cAiMesh* body);
	virtual void Exit(cAiMesh* body);
};