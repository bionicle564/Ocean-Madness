#pragma once
#include <glm/vec3.hpp>
#include "cAiMesh.h"

class cRainDrop
{
public:
	glm::vec3 position;

	cRainDrop();

	void Update(float dt);
};