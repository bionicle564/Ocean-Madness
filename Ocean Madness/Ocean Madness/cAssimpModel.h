#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class cAssimpModel
{
public:
	cAssimpModel(void);
	virtual ~cAssimpModel(void);

	bool LoadModelFromFile(const std::string& file);

	unsigned int Flags(void) { return this->m_pScene->mFlags; }
	aiNode* GetRootNode(void) { return this->m_pScene->mRootNode; }

	bool HasAnimations(void) { return this->m_pScene->HasAnimations(); }
	bool HasCameras(void) { return this->m_pScene->HasCameras(); }
	bool HasLights(void) { return this->m_pScene->HasLights(); }
	bool HasMaterials(void) { return this->m_pScene->HasMaterials(); }
	bool HasMeshes(void) { return this->m_pScene->HasMeshes(); }
	bool HasTextures(void) { return this->m_pScene->HasTextures(); }

	unsigned int GetNumAnimations(void) { return this->m_pScene->mNumAnimations; }
	unsigned int GetNumCameras(void) { return this->m_pScene->mNumCameras; }
	unsigned int GetNumLights(void) { return this->m_pScene->mNumLights; }
	unsigned int MetNumMaterials(void) { return this->m_pScene->mNumMaterials; }
	unsigned int GetNumMeshes(void) { return this->m_pScene->mNumMeshes; }
	unsigned int GetNumTextures(void) { return this->m_pScene->mNumTextures; }

	aiAnimation* GetAnimationByID(unsigned int id) { return this->m_pScene->mAnimations[id]; }
	aiCamera* GetCameraByID(unsigned int id) { return this->m_pScene->mCameras[id]; }
	aiLight* GetLightByID(unsigned int id) { return this->m_pScene->mLights[id]; }
	aiMaterial* GetMaterialByID(unsigned int id) { return this->m_pScene->mMaterials[id]; }
	aiMesh* GetMeshByID(unsigned int id) { return this->m_pScene->mMeshes[id]; }
	aiTexture* GetTextureByID(unsigned int id) { return this->m_pScene->mTextures[id]; }

private:
	const aiScene* m_pScene;
	Assimp::Importer* m_pImporter;
};
