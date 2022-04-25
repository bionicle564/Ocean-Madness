#pragma once
#include "cAiMesh.h"
#include <gdpPhysics/interfaces/iPhysicsFactory.h>

struct sCannonBall
{
	~sCannonBall()
	{
		if (body)
		{
			//delete body;
		}
	}
	cAiMesh mesh;
	gdpPhysics::iRigidBody* body;
};