#include "cEnemyBoat.h"
#include <Windows.h>	
#include <process.h>	// Actual thread calls

CRITICAL_SECTION CS_RenderCall;

cAiMesh cEnemyBoat::boatTemplate;


cEnemyBoat::cEnemyBoat(glm::vec3 position)
{
	InitializeCriticalSection(&CS_RenderCall);
	boatMesh = cAiMesh(this->boatTemplate);

	boatMesh.position = position;

	this->position = position;
	this->direction = glm::vec3(1,0,0);
	
	
}

void cEnemyBoat::Update(float dt, GLuint program, glm::vec3 eye, glm::vec3 at)
{
	//line up collider
	//rb->SetOrigin(boatMesh.position, boatMesh.orientation);
	if (timer < 0)
	{
		boatMesh.orientation.y += glm::radians(90.0f);
		timer = 10;
	}
	

	//set direction
	direction.z = -sin(boatMesh.orientation.y);
	direction.x = cos(boatMesh.orientation.y);

	//move in direction
	boatMesh.position += direction * dt * 1.0f;
	this->position = boatMesh.position;



	timer -= dt;
}
