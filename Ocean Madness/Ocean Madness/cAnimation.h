#pragma once
#include <vector>
#include "cBone.h"
#include <algorithm>
#include <map>
#include "untils.h"

class cAiMesh;



struct AssimpNodeData
{
    glm::mat4 transformation;
    std::string name;
    int childrenCount;
    std::vector<AssimpNodeData> children;
};

class cAnimation
{
private:
    std::vector<std::string> namesUsed;
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;

public:
    cAiMesh* model;
	std::vector<cBone> bones;
    aiAnimation* animationNode;
    float duration;
    int ticksPerSecond;

	//cAnimation(cAnimation* from , cAnimation* to, cAiMesh* model);
	cAnimation(aiAnimation* animationNode, cAiMesh* model);
    cAnimation(cAnimation* copyIn);

    void ReadMissingBones(aiAnimation* animation, cAiMesh* model);

    void ReadHeirarchyData(AssimpNodeData& dest, aiNode* src);

    AssimpNodeData& GetRootNode();

    std::map<std::string, BoneInfo>& GetBoneInfo();

    cBone* FindBone(const std::string& name)
    {
        std::vector<cBone>::iterator iter = std::find_if(bones.begin(), bones.end(),
            [&](const cBone& Bone)             {
                return Bone.name == name;
            }
        );
        if (iter == bones.end()) return nullptr;
        else return &(*iter);
    }

  
};