#ifndef _cVAOManager_HG_
#define _cVAOManager_HG_

// Will load the models and place them 
// into the vertex and index buffers to be drawn

#include <string>
#include <map>

// The vertex structure 
//	that's ON THE GPU (eventually) 
// So dictated from THE SHADER
//struct sVertex
//{
//	float x, y, z;		
//	float r, g, b;
//};

//#include "sVertex_XYZ_RGB.h"
#include "sVertex_types.h"


struct sModelDrawInfo
{
	sModelDrawInfo(); 

	std::string meshName;

	unsigned int VAO_ID;

	unsigned int VertexBufferID;
	unsigned int VertexBuffer_Start_Index;
	unsigned int numberOfVertices;

	unsigned int IndexBufferID;
	unsigned int IndexBuffer_Start_Index;
	unsigned int numberOfIndices;
	unsigned int numberOfTriangles;

	// The "local" (i.e. "CPU side" temporary array)
	sVertex_XYZW_RGBA_N_UV_T_B* pVertices;	//  = 0;
//	sVertex_XYZW_RGBA* pVertices;	//  = 0;
//	sVertex_XYZ_RGB* pVertices;	//  = 0;
	// The index buffer (CPU side)
	unsigned int* pIndices;
};


class cVAOManager
{
public:

	bool LoadModelIntoVAO(std::string fileName, 
						  sModelDrawInfo &drawInfo, 
						  unsigned int shaderProgramID);

	// Same as above, but doesn't load into the VAO
	// i.e. you'd call these in two steps, passing the filename in the drawInfo.meshName
	bool LoadPLYModelFromFile(std::string fileName, sModelDrawInfo& drawInfo);
	// Uses a populated drawInfo structure, with the filename in the drawInfo.meshName
	bool LoadModelIntoVAO(sModelDrawInfo& drawInfo, unsigned int shaderProgramID);

	// You could write this is you didn't want the sModelDrawInfo to return by ref
	bool LoadModelIntoVAO(std::string fileName, 
						  unsigned int shaderProgramID);

	// We don't want to return an int, likely
	bool FindDrawInfoByModelName(std::string filename,
								 sModelDrawInfo &drawInfo);

	std::string getLastError(bool bAndClear = true);

	void setFilePath(std::string filePath);
	std::string getFilePath(void);

	// This is modified from GenerateSphericalTextureCoords.cpp file,
	//	specifically that it's using the sModelDrawInfo structure
	enum enumTEXCOORDBIAS {
		POSITIVE_X, POSITIVE_Y, POSITIVE_Z
	};
	void GenerateSphericalTextureCoords(
		enumTEXCOORDBIAS uBias, enumTEXCOORDBIAS vBias,
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);
	// Defaults to POSITIVE_X, POSITIVE_Y
	void GenerateSphericalTextureCoords(
		sModelDrawInfo& drawInfo, bool basedOnNormals, float scale, bool fast);

private:

	std::map< std::string /*model name*/,
		      sModelDrawInfo /* info needed to draw*/ >
		m_map_ModelName_to_VAOID;

	// This will be added to the path when a file is loaded
	std::string m_FilePath;

	std::string m_lastError;
};

#endif	// _cVAOManager_HG_
