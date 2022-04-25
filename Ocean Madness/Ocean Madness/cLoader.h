#pragma once
#include <string>
#include "cMesh.h"

class cLoader
{
public:
	std::vector<cMesh*> LoadMeshFile();
	void SaveMeshToFile(std::vector< cMesh* > meshs);

	void LoadTextureNames();
	void SaveTextureNames(std::vector<std::string> texNames);

};