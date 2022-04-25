#pragma once

#include <glm/mat4x4.hpp>
#include <assimp/matrix4x4.inl>
#include "GLCommon.h"
#include <glm/gtx/quaternion.hpp>

glm::mat4 ai_to_glm(aiMatrix4x4 from);

glm::vec3 ai_to_glm(aiVector3D from);

glm::quat ai_to_glm(aiQuaternion from);


struct BoneInfo
{
    int id;

    int vertexId;

    glm::mat4 offset;

};
