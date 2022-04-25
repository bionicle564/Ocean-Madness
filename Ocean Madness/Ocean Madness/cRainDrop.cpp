#include "cRainDrop.h"

cRainDrop::cRainDrop()
{
	this->position.y = 30;
	this->position.z = 10;
	this->position.x = 10;
}

void cRainDrop::Update(float dt)
{
	this->position.y += -9.8f * dt;

	if (this->position.y < -4)
	{
		this->position.y = 30;
	}
}
