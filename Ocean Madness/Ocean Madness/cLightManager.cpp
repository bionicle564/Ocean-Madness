#include "cLightManager.h"

#include "GLCommon.h"

#include <sstream>
#include <string>
#include <iostream>
#include <fstream>

cLightManager::cLightManager()
{
	// Set all the values of the lights, etc.



}

// This got moved into here (seemed more clear)
/*static*/ 
cLightHelper cLightManager::lightHelper;


sLight::sLight()
{
	// Clear all the values to the unknown ones
	this->position = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	// White light
	this->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// White specular highlight
	this->specular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	this->atten = glm::vec4(0.0f, 0.01f, 0.001f, 100000.0f);

	this->direction = glm::vec4(0.0f, -1.0f, 0.0f, 0.0f);

	// x = lightType, y = inner angle, z = outer angle, w = TBD
	// 0 = pointlight
	// 1 = spot light
	// 2 = directional light
	this->param1.x = 0;	// Spot light
	this->param1.y = 0.0f;
	this->param1.z = 0.0f;
	this->param1.w = 1.0f;	// not used, so set to 1.0f

	// x = 0 for off, 1 for on
	this->param2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);	

	// Here's the uniform locations of the light values in the shader
	// Settting these to -1 which is the "not found" or unknown uniform location
	this->position_uniform_location = -1;
	this->diffuse_uniform_location = -1;
	this->specular_uniform_location = -1;
	this->atten_uniform_location = -1;
	this->direction_uniform_location = -1;
	this->param1_uniform_location = -1;
	this->param2_uniform_location = -1;
}



void cLightManager::TurnOnLight(unsigned int lightNumber)
{
	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
	{
		this->theLights[lightNumber].param2.x = 1.0f;
	}
	return;
}

void cLightManager::TurnOffLight(unsigned int lightNumber)
{
	if (lightNumber < cLightManager::NUMBER_OF_LIGHTS)
	{
		this->theLights[lightNumber].param2.x = 0.0f;
	}
	return;
}


// This sets up the initial uniform locations from the shader
void cLightManager::SetUpUniformLocations(unsigned int shaderProgram)
{
	




	for (unsigned int lightIndex = 0; lightIndex != NUMBER_OF_LIGHTS; lightIndex++)
	{
		std::stringstream ssLight;
		ssLight << "theLights[" << lightIndex << "].";

		std::string sLightUniformName = ssLight.str() + "position";
		this->theLights[lightIndex].position_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].position_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "diffuse";
		this->theLights[lightIndex].diffuse_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].diffuse_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "specular";
		this->theLights[lightIndex].specular_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].specular_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "atten";
		this->theLights[lightIndex].atten_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].atten_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "direction";
		this->theLights[lightIndex].direction_uniform_location =	glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].direction_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "param1";
		this->theLights[lightIndex].param1_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].param1_uniform_location << std::endl;

		sLightUniformName = ssLight.str() + "param2";
		this->theLights[lightIndex].param2_uniform_location = glGetUniformLocation(shaderProgram, sLightUniformName.c_str() );
		std::cout << sLightUniformName << " = " << this->theLights[lightIndex].param2_uniform_location << std::endl;

		std::cout << std::endl;

	}//for (unsigned int lightIndex

	return;
}

// Copies the values from the array into the shader
void cLightManager::CopyLightInfoToShader(unsigned int shaderProgram)
{
	glUseProgram(shaderProgram);
	for (unsigned int lightIndex = 0; lightIndex != NUMBER_OF_LIGHTS; lightIndex++)
	{
		glUniform4f(this->theLights[lightIndex].position_uniform_location,
			this->theLights[lightIndex].position.x,
			this->theLights[lightIndex].position.y,
			this->theLights[lightIndex].position.z,
			this->theLights[lightIndex].position.w);

		glUniform4f(this->theLights[lightIndex].diffuse_uniform_location,
			this->theLights[lightIndex].diffuse.x,
			this->theLights[lightIndex].diffuse.y,
			this->theLights[lightIndex].diffuse.z,
			this->theLights[lightIndex].diffuse.w);

		glUniform4f(this->theLights[lightIndex].specular_uniform_location,
			this->theLights[lightIndex].specular.x,
			this->theLights[lightIndex].specular.y,
			this->theLights[lightIndex].specular.z,
			this->theLights[lightIndex].specular.w);

		glUniform4f(this->theLights[lightIndex].atten_uniform_location,
			this->theLights[lightIndex].atten.x,
			this->theLights[lightIndex].atten.y,
			this->theLights[lightIndex].atten.z,
			this->theLights[lightIndex].atten.w);

		glUniform4f(this->theLights[lightIndex].direction_uniform_location,
			this->theLights[lightIndex].direction.x,
			this->theLights[lightIndex].direction.y,
			this->theLights[lightIndex].direction.z,
			this->theLights[lightIndex].direction.w);

		glUniform4f(this->theLights[lightIndex].param1_uniform_location,
			this->theLights[lightIndex].param1.x,
			this->theLights[lightIndex].param1.y,
			this->theLights[lightIndex].param1.z,
			this->theLights[lightIndex].param1.w);

		glUniform4f(this->theLights[lightIndex].param2_uniform_location,
			this->theLights[lightIndex].param2.x,
			this->theLights[lightIndex].param2.y,
			this->theLights[lightIndex].param2.z,
			this->theLights[lightIndex].param2.w);
	}//for (unsigned int lightIndex...



	return;
}

void cLightManager::SaveLightInformationToFile(std::string fileName)
{
	std::ofstream file("lights.txt");

	file << NUMBER_OF_LIGHTS << std::endl;

	for (int i = 0; i < NUMBER_OF_LIGHTS; i++)
	{
		sLight light = theLights[i];

		file << light.diffuse.r << " " << light.diffuse.g << " " << light.diffuse.b << " " << light.diffuse.a << " "
			<< light.atten.x << " " << light.atten.y << " " << light.atten.z << " " << light.atten.w << " "
			<< light.direction.x << " " << light.direction.y << " " << light.direction.z << " " << light.direction.w << " "
			<< light.param1.x << " " << light.param1.y << " " << light.param1.z << " " << light.param1.w << " "
			<< light.param2.x << " " << light.param2.y << " " << light.param2.z << " " << light.param2.w << " "
			<< light.position.x << " " << light.position.y << " " << light.position.z << " " << light.position.w << " "
			<< light.specular.r << " " << light.specular.g << " " << light.specular.b << " " << light.specular.a << std::endl;
	}
	file.close();
}

void cLightManager::LoadLightInformationFromFile(std::string fileName)
{
	std::ifstream file("lights.txt");
	int length;
	file >> length;

	for (int i = 0; i < length; i++)
	{
		file >> theLights[i].diffuse.r >> theLights[i].diffuse.g >> theLights[i].diffuse.b >> theLights[i].diffuse.a
			>> theLights[i].atten.x >> theLights[i].atten.y >> theLights[i].atten.z >> theLights[i].atten.w
			>> theLights[i].direction.x >> theLights[i].direction.y >> theLights[i].direction.z >> theLights[i].direction.w
			>> theLights[i].param1.x >> theLights[i].param1.y >> theLights[i].param1.z >> theLights[i].param1.w
			>> theLights[i].param2.x >> theLights[i].param2.y >> theLights[i].param2.z >> theLights[i].param2.w
			>> theLights[i].position.x >> theLights[i].position.y >> theLights[i].position.z >> theLights[i].position.w
			>> theLights[i].specular.r >> theLights[i].specular.g >> theLights[i].specular.b >> theLights[i].specular.a;
	}
	file.close();
}