#include "untils.h"

glm::mat4 ai_to_glm(aiMatrix4x4 from)
{
    glm::mat4 to = glm::mat4(1.0f);
    to[0][0] = (GLfloat)from.a1;
    to[0][1] = (GLfloat)from.b1;
    to[0][2] = (GLfloat)from.c1;
    to[0][3] = (GLfloat)from.d1;
    to[1][0] = (GLfloat)from.a2;
    to[1][1] = (GLfloat)from.b2;
    to[1][2] = (GLfloat)from.c2;
    to[1][3] = (GLfloat)from.d2;
    to[2][0] = (GLfloat)from.a3;
    to[2][1] = (GLfloat)from.b3;
    to[2][2] = (GLfloat)from.c3;
    to[2][3] = (GLfloat)from.d3;
    to[3][0] = (GLfloat)from.a4;
    to[3][1] = (GLfloat)from.b4;
    to[3][2] = (GLfloat)from.c4;
    to[3][3] = (GLfloat)from.d4;
    return to;
};

glm::vec3 ai_to_glm(aiVector3D from)
{
    glm::vec3 to(0);
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;

    return to;

}

glm::quat ai_to_glm(aiQuaternion from)
{
    glm::quat to;
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
    to.w = from.w;

    return to;

}

