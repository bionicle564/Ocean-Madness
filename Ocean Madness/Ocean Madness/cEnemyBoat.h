#pragma once
#include "cAiMesh.h"
#include <gdpPhysics/interfaces/iPhysicsWorld.h>

class cEnemyBoat
{
private:

	
	
	float timer = 10;
	
	glm::vec3 direction;

public:
	glm::vec3 FORWARD = glm::vec3(1, 0, 0);
	static cAiMesh boatTemplate;
	cAiMesh boatMesh;
	glm::vec3 position;
	float angle;

	cEnemyBoat(glm::vec3 position);
	

	void Update(float dt, GLuint program, glm::vec3 eye, glm::vec3 at);


};