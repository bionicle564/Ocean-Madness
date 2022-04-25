#pragma once
//This class is heavaly inspired by https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation tutorial
//as well as the cAnimation and cAnimator

#include <assimp/scene.h>
#include <assimp/anim.h>
#include <string>
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

struct KeyPosition
{
    glm::vec3 position;
    float timeStamp;
};

struct KeyRotation
{
    glm::quat orientation;
    float timeStamp;
};

struct KeyScale
{
    glm::vec3 scale;
    float timeStamp;
};

class cBone
{
private:
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    /*figures out which position keys to interpolate b/w and performs the interpolation
    and returns the translation matrix*/
    glm::mat4 InterpolatePosition(float animationTime)
    {
        if (1 == numPositions)
            return glm::translate(glm::mat4(1.0f), positions[0].position);

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(positions[p0Index].timeStamp,
            positions[p1Index].timeStamp, animationTime);
        glm::vec3 finalPosition = glm::mix(positions[p0Index].position,
            positions[p1Index].position, scaleFactor);

        glm::mat4 rtn = glm::translate(glm::mat4(1.0f), finalPosition);

        localPosition = finalPosition;

        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    /*figures out which rotations keys to interpolate b/w and performs the interpolation
    and returns the rotation matrix*/
    glm::mat4 InterpolateRotation(float animationTime)
    {
        if (1 == numRotations)
        {
            glm::qua<float> rotation = glm::normalize(rotations[0].orientation);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(rotations[p0Index].timeStamp,
            rotations[p1Index].timeStamp, animationTime);
        glm::quat finalRotation = glm::slerp(rotations[p0Index].orientation,
            rotations[p1Index].orientation, scaleFactor);
        finalRotation = glm::normalize(finalRotation);

        localDirection = finalRotation;

        return glm::toMat4(finalRotation);
    }

    /*figures out which scaling keys to interpolate b/w and performs the interpolation
    and returns the scale matrix*/
    glm::mat4 InterpolateScaling(float animationTime)
    {
        if (1 == numScalings)
            return glm::scale(glm::mat4(1.0f), scales[0].scale);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(scales[p0Index].timeStamp,
            scales[p1Index].timeStamp, animationTime);
        glm::vec3 finalScale = glm::mix(scales[p0Index].scale, scales[p1Index].scale
            , scaleFactor);

        localScale = finalScale;

        return glm::scale(glm::mat4(1.0f), finalScale);
    }

public:
    std::vector<KeyPosition> positions;
    std::vector<KeyRotation> rotations;
    std::vector<KeyScale> scales;
    int numPositions;
    int numRotations;
    int numScalings;

    glm::vec3 localPosition;
    glm::quat localDirection;
    glm::vec3 localScale;


    glm::mat4 localTransform;
    std::string name;
    int ID;


	cBone(std::string name, int ID, aiNodeAnim* channel);

	void Update(float animationTime);

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    int GetScaleIndex(float animationTime);

    bool operator== (cBone rhs)
    {
        return this->name == rhs.name;
    }
};