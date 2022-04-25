#pragma once
#include <gdpPhysics/interfaces/iPhysicsWorld.h>
#include <gdpPhysics/interfaces/iPhysicsFactory.h>
#include "cAiMesh.h"
#include "sCannonBall.h"

#include <vector>

class cBoatWorld
{
private:
	//gdpPhysics::iPhysicsWorld& world;
	gdpPhysics::iPhysicsFactory* factory;
	gdpPhysics::iPhysicsWorld* world;

	gdpPhysics::iRigidBody* boat;
	gdpPhysics::iRigidBody* portRail;
	gdpPhysics::iRigidBody* starboardRail;
	gdpPhysics::iRigidBody* backWall;
	gdpPhysics::iRigidBody* frontWall;

	gdpPhysics::iRigidBody* wheelWall;

	//std::vector<gdpPhysics::iRigidBody*> projectiles;
	
	

	glm::vec3 portRailOffset;
	glm::vec3 starboardRailOffset;
	glm::vec3 frontWallOffset;
	glm::vec3 backWallOffset;

	glm::vec3 wheelWallOffset;
	

public:
	std::vector<sCannonBall> projectiles;
	cAiMesh cannonBall;
	std::vector<cAiMesh> meshList;

	gdpPhysics::iRigidBody* player;
	glm::vec3 boatPosition;
	glm::vec3 boatOrientation;

	glm::vec3 playerPosition;
	glm::vec3 playerOrientation;


	cBoatWorld();
	~cBoatWorld();

	void Init(glm::vec3 boatPosition, glm::vec3 boatOrientaion);
	
	glm::vec3 GetBoatPosition();
	glm::vec3 GetPlayerPosition();

	void Update(float dt, GLuint program, glm::vec3 eye, glm::vec3 at);

	void ShootCannonBall(glm::vec3 from, glm::vec3 direction);

	void ShootChainShot(glm::vec3 from, glm::vec3 direction);

	gdpPhysics::iPhysicsWorld* GetWorld() { return world; }
};