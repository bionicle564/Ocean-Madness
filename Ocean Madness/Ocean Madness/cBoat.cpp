#include "cBoat.h"

cBoat::cBoat(iMediator* mediator, GLuint* programs)
{
	ocean.SetFolderPath("assets\\models\\");
	ocean.LoadModel("dense_tile.fbx");
	ocean.LoadIntoVAO(programs[1]);
	ocean.textureNumber = programs[2];
	ocean.instanced = true;
	ocean.instanceCount = 90000;
	ocean.position = glm::vec3(-300, 0, 300);
	ocean.orientation = glm::vec3(glm::radians(-90.0f), glm::radians(0.0f), glm::radians(0.0f));

	boat.SetFolderPath("assets\\models\\");
	boat.LoadModel("ship.fbx");
	boat.LoadIntoVAO(programs[0]);
	boat.scale = glm::vec3(.01);
	boat.position = glm::vec3(0, 4, 0);

	lastStep = boat.position;
	lastStep.y = 0;


	//starboardCannon;
	starboardCannon.SetFolderPath("assets\\models\\");
	starboardCannon.LoadModel("cannon.fbx");
	starboardCannon.LoadIntoVAO(programs[0]);
	starboardCannon.scale = glm::vec3(.6, .6, .6);
	starboardCannon.position.z = 3.5;
	starboardCannon.position.y = 3.5;

	//portCannon;
	portCannon.SetFolderPath("assets\\models\\");
	portCannon.LoadModel("cannon.fbx");
	portCannon.LoadIntoVAO(programs[0]);
	portCannon.scale = glm::vec3(.6, .6, -.6);
	portCannon.position.z = -3.5;
	portCannon.position.y = 3.5;

	ammoPile.SetFolderPath("assets\\models\\");
	ammoPile.LoadModel("ball_tray.fbx");
	ammoPile.LoadIntoVAO(programs[0]);
	ammoPile.position.y = 4;
	ammoPile.position.x = 10;
	ammoPile.orientation = glm::vec3(glm::radians(-90.0f), glm::radians(90.0f), glm::radians(0.0f));

	wheel.SetFolderPath("assets\\models\\");
	wheel.LoadModel("ship_wheel.fbx");
	wheel.LoadIntoVAO(programs[0]);
	wheel.position.y = 4;
	wheel.position.x = -6;
	//wheel.orientation = glm::vec3(glm::radians(-90.0f), glm::radians(90.0f), glm::radians(0.0f));


	this->SetReciever(mediator);
}

bool cBoat::SetReciever(iMediator* pTheReciever)
{
	this->mediator = pTheReciever;
	return true;
}

bool cBoat::RecieveMessage(sMessage theMessage)
{
	return false;
}

bool cBoat::RecieveMessage(sMessage theMessage, sMessage& messageReply)
{
	return false;
}

void cBoat::Update(float deltaTime)
{
	for (int i = 0; i < projectileMeshes.size(); i++)
	{
		if (projectileMeshes[i].position.y < -4)
		{
			projectileMeshes.erase(projectileMeshes.begin() + i);
			i--;
		}
	}
	//ocean.position = boat.position + glm::vec3(-300, 0, 300);

	currentStep = boat.position;
	currentStep.y = 0;
	if (glm::distance(currentStep, lastStep) > 3)
	{
		//ocean.position = boat.position + glm::vec3(-300, 0, 300);
		lastStep = boat.position;
		lastStep.y = 0;
	}
	

	glm::vec3 cannonNewOffset(0);

	cannonNewOffset.z = -sin(boat.orientation.y + glm::radians(-90.0f)) * 3.5;
	cannonNewOffset.x = cos(boat.orientation.y + glm::radians(-90.0f)) * 3.5;
	cannonNewOffset.y = 3.5;

	starboardCannon.position = boat.position + cannonNewOffset; //+ glm::vec3(0, 3.5, 3.5);
	starboardCannon.orientation.y = boat.orientation.y + starboardCannon.helperValue;

	cannonNewOffset.z = -sin(boat.orientation.y + glm::radians(-90.0f)) * -3.5;
	cannonNewOffset.x = cos(boat.orientation.y + glm::radians(-90.0f)) * -3.5;

	portCannon.position = boat.position + cannonNewOffset;// glm::vec3(0, 3.5, -3.5);
	portCannon.orientation.y = boat.orientation.y + portCannon.helperValue;

	

	cannonNewOffset.z = -sin(boat.orientation.y) * 10;
	cannonNewOffset.x = cos(boat.orientation.y) * 10;
	ammoPile.position = boat.position + cannonNewOffset;// glm::vec3(10, 4, 0);
	ammoPile.orientation.y = boat.orientation.y + glm::radians(-90.0f);

	cannonNewOffset.y = 4;
	cannonNewOffset.z = -sin(boat.orientation.y) * -6;
	cannonNewOffset.x = cos(boat.orientation.y) * -6;
	wheel.position = boat.position + cannonNewOffset;// glm::vec3(-6, 4, 0);
	wheel.orientation.y = boat.orientation.y;
}

void cBoat::Render(GLuint* programs, glm::vec3 eye, glm::vec3 at)
{
	for (int i = 0; i < projectileMeshes.size(); i++)
	{
		projectileMeshes[i].Render(programs[0], eye, at);
	}
	boat.Render(programs[0],eye,at);
	starboardCannon.Render(programs[0],eye,at);
	portCannon.Render(programs[0],eye,at);
	ammoPile.Render(programs[0], eye, at);
	wheel.Render(programs[0], eye, at);

	ocean.Render(programs[1],eye,at);
	
}
