#include "cLoader.h"
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include "globalThings.h"

std::vector<cMesh*> cLoader::LoadMeshFile()
{
    std::vector<cMesh*> listOfMeshes;
    std::ifstream file("objects.txt");


    if (!file)
    {
        std::cout << "File not found" << std::endl;
        return listOfMeshes;
    }

    int length;
    file >> length;

    for (int i = 0; i < length; i++)
    {
        cMesh* curPtr = new cMesh();


        file >> curPtr->meshName 
            >> curPtr->textureNames[0] >> curPtr->textureRatios[0]
            >> curPtr->textureNames[1] >> curPtr->textureRatios[1]
            >> curPtr->textureNames[2] >> curPtr->textureRatios[2]
            >> curPtr->textureNames[3] >> curPtr->textureRatios[3]
            >> curPtr->positionXYZ.x >> curPtr->positionXYZ.y >> curPtr->positionXYZ.z
            >> curPtr->orientationXYZ.x >> curPtr->orientationXYZ.y >> curPtr->orientationXYZ.z
            >> curPtr->scale >> curPtr->bIsWireframe
            >> curPtr->bDontLight
            >> curPtr->bUseWholeObjectDiffuseColour >> curPtr->wholeObjectDiffuseRGBA.r >> curPtr->wholeObjectDiffuseRGBA.g >> curPtr->wholeObjectDiffuseRGBA.b >> curPtr->wholeObjectDiffuseRGBA.a
            >> curPtr->wholeObjectShininess_SpecPower
            >> curPtr->wholeObjectSpecularRGB.r >> curPtr->wholeObjectSpecularRGB.g >> curPtr->wholeObjectSpecularRGB.b;

        for (int j = 0; j < 4; j++)
        {
            if (curPtr->textureNames[j] == "n/a")
            {
                curPtr->textureNames[j] = "";
            }
        }

        listOfMeshes.push_back(curPtr);

    }
    file.close();


    return listOfMeshes;
}

void cLoader::SaveMeshToFile(std::vector<cMesh*> meshs)
{
    std::ofstream file("objects.txt");

    file << meshs.size() << std::endl;

    for (int i = 0; i < meshs.size(); i++)
    {
        cMesh* curPtr = meshs[i];

        std::stringstream textureLine;
        for (int j = 0; j < 4; j++)
        {
            if (curPtr->textureNames[j] != "")
            {
                textureLine << curPtr->textureNames[j] << " " << curPtr->textureRatios[j] << " ";
            }
            else
            {
                textureLine << "n/a" << " " << curPtr->textureRatios[j] << " ";
            }
        }

        file << curPtr->meshName << " " 
            << textureLine.str()
            << curPtr->positionXYZ.x << " " << curPtr->positionXYZ.y << " " << curPtr->positionXYZ.z << " "
            << curPtr->orientationXYZ.x << " " << curPtr->orientationXYZ.y << " " << curPtr->orientationXYZ.z << " "
            << curPtr->scale << " " << curPtr->bIsWireframe << " "
            << curPtr->bDontLight << " "
            << curPtr->bUseWholeObjectDiffuseColour << " " << curPtr->wholeObjectDiffuseRGBA.r << " " << curPtr->wholeObjectDiffuseRGBA.g << " " << curPtr->wholeObjectDiffuseRGBA.b << " " << curPtr->wholeObjectDiffuseRGBA.a << " "
            << curPtr->wholeObjectShininess_SpecPower << " "
            << curPtr->wholeObjectSpecularRGB.r << " " << curPtr->wholeObjectSpecularRGB.g << " " << curPtr->wholeObjectSpecularRGB.b << std::endl;

    }
    file.close();
}



void cLoader::LoadTextureNames()
{
    std::vector<std::string> listOfTextures;
    std::ifstream file("textures.txt");


    if (!file)
    {
        std::cout << "File not found" << std::endl;
        return;
    }
    
    int length;
    file >> length;

    std::string line;
    for (int i = 0; i < length; i++)
    {
        file >> line;
        listOfTextures.push_back(line);
    }

    g_pTextureManager->SetBasePath("assets/textures");
    for (int i = 0; i < length; i++)
    {
        if (!g_pTextureManager->HasTexture(listOfTextures[i]))
        {
            bool thing = g_pTextureManager->Create2DTextureFromBMPFile(listOfTextures[i], true);
        }
    }


}

void cLoader::SaveTextureNames(std::vector<std::string> texNames)
{
    std::ofstream file("textures.txt");

    file << texNames.size();

    for (int i = 0; i < texNames.size(); i++)
    {
        file << texNames[i] << std::endl;
    }

    file.close();
}
