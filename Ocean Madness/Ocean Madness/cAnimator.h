#pragma once
#include "cAnimation.h"
#include <map>

class cAnimator
{
private:
    float currentTime;
    float deltaTime;
    std::map<std::string, int> names;
public:
    cAnimation* currentAnimation;
    bool loop = true;
    bool isDone = false;

    std::vector<glm::mat4> finalBoneMatrices;


    cAnimator(cAnimation* animation);

    void UpdateAnimation(float deltaTime);

    void PlayAnimation(cAnimation* animation);

    void CalculateBoneTransform(AssimpNodeData* node, glm::mat4 parentTransform);

};