#pragma once

class cAiMesh;

class cMeshState
{
public:
	virtual ~cMeshState() {}

	virtual void Enter(cAiMesh* body) = 0;
	virtual void Update(cAiMesh* body, float deltaTime) = 0;
	virtual void Toggle(cAiMesh* body) = 0;
	virtual void Exit(cAiMesh* body) = 0;
};