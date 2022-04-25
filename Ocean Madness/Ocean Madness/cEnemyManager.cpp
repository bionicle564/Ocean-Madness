#include "cEnemyManager.h"

#include <time.h>


DWORD WINAPI EnemyUpdateFunction(LPVOID lpParameter)
{
	cEnemyManager::sEnemyThreadDesc* desc = (cEnemyManager::sEnemyThreadDesc*)lpParameter;


	Sleep(0);
	while (desc->runThread)
	{
		for (int i = 0; i < desc->boats->size(); i++)
		{
			//cPlant* tempPlant = desc->plants->RealAt(i);
			desc->boats->at(i).Update(*desc->deltaTime,desc->program, *desc->camaeraEye, *desc->camaeraAt);


			for (int j = 0; j < desc->projectiles->size(); j++)
			{
				//check if the distances are less than the sum of the radius of both objects
				if (glm::distance(desc->boats->at(i).position, desc->projectiles->at(j).mesh.position) < (9 + 1))
				{
					desc->projectiles->at(j).body->SetOrigin(glm::vec3(0,-6,0), glm::vec3(0));
				  	int asdfasdf = 9;
				  	desc->boats->erase(desc->boats->begin() + i);
					i--;
					break;
				}
			}

		}
		Sleep(10);
	}
	delete desc;
	return 0;

}


cEnemyManager::cEnemyManager(GLuint program)
{
    this->boatTemplate.SetFolderPath("assets\\models\\");
    this->boatTemplate.LoadModel("ship.fbx");
    this->boatTemplate.LoadIntoVAO(program);
    this->boatTemplate.scale = glm::vec3(.01f);
    this->boatTemplate.position.z = 10;

	cEnemyBoat::boatTemplate = cAiMesh(boatTemplate);

	this->program = program;

}

void cEnemyManager::SetUpBoats(int amount)
{
	for (int i = 0; i < amount; i++)
	{
		int x;
		int y;
		
		x = rand() % (600 + 10) - 300;
		y = rand() % (600 + 10) - 300;
		

		//cAiMesh boat = cAiMesh(boatTemplate);
		
		boats.push_back(cEnemyBoat(glm::vec3(x,0,y)));
		
		
	}

	sEnemyThreadDesc* desc = new sEnemyThreadDesc();
	desc->boats = &this->boats;
	desc->deltaTime = &this->deltaTime;
	desc->projectiles = this->projectiles;
	desc->camaeraEye = &this->camaeraEye;
	desc->camaeraAt = &this->camaeraAt;
	desc->runThread = &this->runThreads;
	desc->program = this->program;

	//EnemyUpdateFunction
	desc->hWorkerThread = CreateThread(NULL, 0, EnemyUpdateFunction, desc, 0, &(desc->workerThreadID));

}

void cEnemyManager::AddBoat()
{
}

void cEnemyManager::Update(float dt, glm::vec3 eye, glm::vec3 at)
{
	for (int i = 0; i < boats.size(); i++)
	{
		boats[i].boatMesh.Render(program, eye, at);
	}

	this->deltaTime = dt;
	this->camaeraEye = eye;
	this->camaeraAt = at;
}
