#include "cAnimation.h"
#include "cAiMesh.h"
#include "untils.h"



cAnimation::cAnimation(aiAnimation* animationNode, cAiMesh* model)
{
    this->model = model;
    this->animationNode = animationNode;
	duration = animationNode->mDuration;
	ticksPerSecond = animationNode->mTicksPerSecond;

	ReadMissingBones(animationNode, model);
	ReadHeirarchyData(rootNode, model->root->mRootNode);
}

cAnimation::cAnimation(cAnimation* copyIn)
{
    this->model = copyIn->model;
    this->boneInfoMap = copyIn->boneInfoMap;
    this->bones = copyIn->bones;
    this->animationNode = copyIn->animationNode;

    this->namesUsed = copyIn->namesUsed;
    this->duration = copyIn->duration;

    //ReadMissingBones(animationNode, copyIn->model);
    ReadHeirarchyData(rootNode, copyIn->model->root->mRootNode);

}

void cAnimation::ReadMissingBones(aiAnimation* animation, cAiMesh* model)
{
    int size = animation->mNumChannels;

    std::map<std::string, BoneInfo>& boneInfoMap = model->bones;//getting m_BoneInfoMap from Model class
    int& boneCount = model->nBones; //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];
        
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            
            boneCount++;
        }
        cBone newBone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel);
        //if (std::find(bones.begin(), bones.end(), newBone) == bones.end())
        

        bones.push_back(newBone);
        
    }

    this->boneInfoMap = boneInfoMap;
}

void cAnimation::ReadHeirarchyData(AssimpNodeData& dest, aiNode* src)
{
    dest.name = src->mName.data;
    dest.transformation = ai_to_glm(src->mTransformation);
    if (dest.name == "RootNode")
    {
        dest.childrenCount = 1;
    }
    //else
    {
        dest.childrenCount = src->mNumChildren;
    }
    
    //aiBone* test = (aiBone*)src;

    

    for (int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHeirarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

AssimpNodeData& cAnimation::GetRootNode()
{
    return rootNode;
}

std::map<std::string, BoneInfo>& cAnimation::GetBoneInfo()
{
    return boneInfoMap;

}
