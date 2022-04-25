#pragma once
#include <string>
#include <assimp/Vertex.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <vector>
#include <map>

#include "GLCommon.h"
#include "sVertex_types.h"
#include "cBone.h"
#include "untils.h"

#include "cAnimator.h"
#include "cMeshState.h"

#include <glm/vec3.hpp>

struct Texture
{
	unsigned int id;
	std::string type;
};

struct sAiModelDrawInfo
{
	sAiModelDrawInfo() {}

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	unsigned int nBones;

	glm::vec4 specInfo;
	
	
	std::vector<aiMatrix4x4> boneMatrices;
	std::vector<glm::mat4> boneMats;

	//add the info into the first available spot
	void AddBoneInfo(int vertexId, int boneId, float weight)
	{
		
		if (pVertices[vertexId].bId1 == -1)
		{
			pVertices[vertexId].bId1 = boneId;
			pVertices[vertexId].bWeight1 = weight;
			return;
		}
		else if (pVertices[vertexId].bId2 == -1)
		{
			pVertices[vertexId].bId2 = boneId;
			pVertices[vertexId].bWeight2 = weight;
			return;
		}
		else if (pVertices[vertexId].bId3 == -1)
		{
			pVertices[vertexId].bId3 = boneId;
			pVertices[vertexId].bWeight3 = weight;
			return;
		}
		else if (pVertices[vertexId].bId4 == -1)
		{
			pVertices[vertexId].bId4 = boneId;
			pVertices[vertexId].bWeight4 = weight;
			return;
		}
		
	}
	



	// The "local" (i.e. "CPU side" temporary array)
	std::vector<sVertex_XYZW_RGBA_N_UV_T_B> pVertices;	//  = 0;
//	sVertex_XYZW_RGBA* pVertices;	//  = 0;
//	sVertex_XYZ_RGB* pVertices;	//  = 0;
	// The index buffer (CPU side)
	std::vector<unsigned int> pIndices;
};



class cAiMesh
{
private:
	std::string folderPath;
	cMeshState* state;

	Assimp::Importer* importer;


public:
	GLint textureNumber = -1;
	GLuint cubeMapTexNumber;

	float helperValue = 0;
	int instanceCount;
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 scale;

	bool isRelective;
	bool instanced = false;

	int nBones;
	std::map<std::string, BoneInfo> bones;

	const aiScene* root;
	std::vector<sAiModelDrawInfo> drawInfo;

	std::vector<cAnimation*> animations;
	cAnimator* controller;

	cAiMesh();
	cAiMesh(const cAiMesh& copyIn);
	~cAiMesh();

	bool LoadModel(std::string fileName);

	bool LoadIntoVAO(GLuint shaderProgramID);

	void SetFolderPath(std::string path);

	void UpdateMatricies();

	void PlayAnimation(int animationNumber);

	//take the current positions, then blend to the first frame of the animation, then play as normal
	cAnimation* BlendAnimation(int animationNumber);

	void Update(float deltaTime);

	void SetState(cMeshState* newState);
	void Toggle();

	void Render(GLuint shaderProgramID,glm::vec3 eye, glm::vec3 at);
};