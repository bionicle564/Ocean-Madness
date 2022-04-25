#include "cBone.h"
#include "untils.h"

cBone::cBone(std::string name, int ID, aiNodeAnim* channel)
{
	this->name = name;
	this->ID = ID;
	this->localTransform = glm::mat4(1.0f);

	
	
	 
	numPositions = channel->mNumPositionKeys;

	for (int i = 0; i < numPositions; i++)
	{
		aiVector3D aiPosition = channel->mPositionKeys[i].mValue;
		float time = channel->mPositionKeys[i].mTime;
		KeyPosition data;
		data.position = ai_to_glm(aiPosition);
		data.timeStamp = time;
		positions.push_back(data);
	}

	numRotations = channel->mNumRotationKeys;

	for (int i = 0; i < numRotations; i++)
	{
		aiQuaternion aiPosition = channel->mRotationKeys[i].mValue;
		float time = channel->mRotationKeys[i].mTime;
		KeyRotation data;
		data.orientation= ai_to_glm(aiPosition);
		data.timeStamp = time;
		rotations.push_back(data);
	}

	numScalings = channel->mNumScalingKeys;

	for (int i = 0; i < numScalings; i++)
	{
		aiVector3D aiPosition = channel->mScalingKeys[i].mValue;
		float time = channel->mScalingKeys[i].mTime;
		KeyScale data;
		data.scale = ai_to_glm(aiPosition);
		data.timeStamp = time;
		scales.push_back(data);
	}

}

void cBone::Update(float animationTime)
{
	if (this->name == "Body")
	{
		int pause = 0;
	}
	glm::mat4 translation = InterpolatePosition(animationTime);
	
	glm::mat4 rotation = InterpolateRotation(animationTime);
	glm::mat4 scale = InterpolateScaling(animationTime);
	localTransform = translation * rotation * scale;
}

int cBone::GetPositionIndex(float animationTime)
{
	for (int index = 0; index < numPositions - 1; index++)
	{
		if (animationTime < positions[index + 1].timeStamp)
			return index;
	}
}

int cBone::GetRotationIndex(float animationTime)
{
	for (int index = 0; index < numRotations - 1; index++)
	{
		if (animationTime < rotations[index + 1].timeStamp)
			return index;
	}
}

int cBone::GetScaleIndex(float animationTime)
{
	for (int index = 0; index < numScalings - 1; index++)
	{
		if (animationTime < scales[index + 1].timeStamp)
			return index;
	}
}
