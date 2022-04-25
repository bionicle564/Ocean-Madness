#include "cWeather.h"
#include "globalThings.h"
#include <sstream>

DWORD WINAPI RainDropUpdateFunction(LPVOID lpParameter)
{
	cWeather::sWeatherThreadDesc* desc = (cWeather::sWeatherThreadDesc*)lpParameter;


	Sleep(0);
	while (desc->runThread)
	{
		for (int i = 0; i < desc->drops->size(); i++)
		{
			//cPlant* tempPlant = desc->plants->RealAt(i);
			desc->drops->at(i).Update(*desc->deltaTime);

            //glUniform4f(desc->uniforms->at(i), desc->drops->at(i).position.x, desc->drops->at(i).position.y, desc->drops->at(i).position.z,1);
			
		}
		Sleep(10);
	}
	delete desc;
	return 0;

}


cWeather::cWeather(GLuint program, int nDrops)
{
    dropMesh.SetFolderPath("assets\\models\\");
    dropMesh.LoadModel("rain.fbx");
    dropMesh.LoadIntoVAO(program);
    dropMesh.position.y = 0;
    dropMesh.position.x = 0;
    dropMesh.orientation.x = glm::radians(-90.0f);
    dropMesh.isRelective = true;
    dropMesh.instanced = true;
    dropMesh.instanceCount = nDrops;
    dropMesh.cubeMapTexNumber = ::g_pTextureManager->getTextureIDFromName("daySky");

    this->numberOfDrops = nDrops;
    this->program = program;
}

void cWeather::Init()
{
    for (int i = 0; i < this->numberOfDrops; i++)
    {
        drops.push_back(cRainDrop());
        drops[i].position.z = 0;
        drops[i].position.x = 0;

        std::stringstream ss;
        ss << "RainOffsets[" << i << "]";

        uniforms.push_back(glGetUniformLocation(this->program, ss.str().c_str()));

    }

    boolUniform = glGetUniformLocation(this->program, "isRain");

    sWeatherThreadDesc* desc = new sWeatherThreadDesc();
    desc->deltaTime = &this->deltaTime;
    desc->drops = &this->drops;
    desc->runThread = &this->runThreads;
    desc->uniforms = &this->uniforms;

    //EnemyUpdateFunction
    desc->hWorkerThread = CreateThread(NULL, 0, RainDropUpdateFunction, desc, 0, &(desc->workerThreadID));


}

void cWeather::Update(float dt, glm::vec3 eye, glm::vec3 at)
{
    this->deltaTime = dt;

    for (int i = 0; i < drops.size(); i++)
    {
        glUniform4f(uniforms[i], drops[i].position.x, drops[i].position.y, drops[i].position.z, 1);

    }


    glUniform1f(this->boolUniform, (float)GL_TRUE);
    
    this->dropMesh.Render(program, eye,at);

    glUniform1f(this->boolUniform, (float)GL_FALSE);
}
