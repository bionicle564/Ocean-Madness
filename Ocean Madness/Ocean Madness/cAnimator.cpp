#include "cAnimator.h"
#include <iostream>

cAnimator::cAnimator(cAnimation* animation)
{
    currentTime = 0.0;
    currentAnimation = currentAnimation;

    finalBoneMatrices.reserve(200);

    for (int i = 0; i < 200; i++)
        finalBoneMatrices.push_back(glm::mat4(1.0f));
}

void cAnimator::UpdateAnimation(float deltaTime)
{
    this->deltaTime = deltaTime;
    if (currentAnimation)
    {
        names.clear();
        currentTime += currentAnimation->ticksPerSecond * deltaTime;
        //currentTime = fmod(currentTime, currentAnimation->duration);

        if (currentTime >= currentAnimation->duration)
        {
            if (!loop)
            {
                isDone = true;
                return;
            }
            currentTime = fmod(currentTime, currentAnimation->duration);
        }
        else
        {
            isDone = false;
        }
        
        //std::cout << this->currentTime << std::endl;

        CalculateBoneTransform(&currentAnimation->GetRootNode(), glm::mat4(1.0f));
    }
    else
    {
        
    }
}

void cAnimator::PlayAnimation(cAnimation* animation)
{
    currentAnimation = animation;
    currentTime = 0.0f;
}

void cAnimator::CalculateBoneTransform(AssimpNodeData* node, glm::mat4 parentTransform)
{
    std::string nodeName = node->name;
    names[nodeName]++;

    glm::mat4 nodeTransform = node->transformation;

    cBone* Bone = currentAnimation->FindBone(nodeName);

    if (Bone)
    {
        Bone->Update(currentTime);
        nodeTransform = Bone->localTransform;
    }

    glm::mat4 globalTransformation = parentTransform * nodeTransform;

    std::map<std::string, BoneInfo>& boneInfoMap = currentAnimation->GetBoneInfo();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end())
    {
        int index = boneInfoMap[nodeName].id;
        glm::mat4 offset = boneInfoMap[nodeName].offset;
        finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (int i = 0; i < node->childrenCount; i++)
    {
        if(names[nodeName] <= 1)
            CalculateBoneTransform(&node->children[i], globalTransformation);
    }
}
