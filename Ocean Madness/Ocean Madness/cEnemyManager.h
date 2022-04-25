#pragma once


#include <Windows.h>	
#include <process.h>	// Actual thread calls

#include "cEnemyBoat.h"
#include "sCannonBall.h"


class cEnemyManager
{
private:
	cAiMesh boatTemplate;
	bool runThreads = true;

	GLuint program;

public:


	struct sEnemyThreadDesc
	{
		std::vector<cEnemyBoat>* boats;
		std::vector<sCannonBall>* projectiles;

		float* deltaTime;
		glm::vec3* camaeraEye;
		glm::vec3* camaeraAt;
		GLuint program;

		bool* runThread;
		HANDLE hWorkerThread;
		DWORD workerThreadID;

	};



	float deltaTime;
	glm::vec3 camaeraEye;
	glm::vec3 camaeraAt;

	std::vector<sCannonBall>* projectiles;
	std::vector<cEnemyBoat> boats;

	cEnemyManager(GLuint program);

	void SetUpBoats(int amount);
	void AddBoat();

	void Update(float dt, glm::vec3 eye, glm::vec3 at);
};