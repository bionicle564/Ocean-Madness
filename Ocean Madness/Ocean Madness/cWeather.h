#pragma once
#include <Windows.h>	
#include <process.h>	// Actual thread calls

#include <vector>
#include "cRainDrop.h"

class cWeather
{
private:
	int numberOfDrops;
	cAiMesh dropMesh;
	bool draw;

	float timer = 50;
	GLuint program;
	bool runThreads = true;

	GLint boolUniform;

	std::vector<GLint> uniforms;

public:

	struct sWeatherThreadDesc
	{
		std::vector<cRainDrop>* drops;
		std::vector<GLint>* uniforms;

		float* deltaTime;
		//glm::vec3* camaeraEye;
		//glm::vec3* camaeraAt;
		//GLuint program;

		bool* runThread;
		HANDLE hWorkerThread;
		DWORD workerThreadID;

	};


	cWeather(GLuint program,int nDrops);

	float deltaTime;
	std::vector<cRainDrop> drops;
	

	void Init();

	void Update(float dt, glm::vec3 eye, glm::vec3 at);

};