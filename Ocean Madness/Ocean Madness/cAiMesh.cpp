#include "cAiMesh.h"
#include <sstream>
#include <assimp/postprocess.h>

#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "untils.h"

#include "cAnimation.h"



aiNode* FindNodeRecursivelyByName(aiNode* root, aiString name)
{
    
    if (root->mName == name)
    {
        return root;
    }
    else
    {
        for (int i = 0; i < root->mNumChildren; i++)
        {
            aiNode* rtn = FindNodeRecursivelyByName(root->mChildren[i], name);
            if (rtn != NULL)
            {
                return rtn;
            }
        }
        return NULL;
    }
}


cAiMesh::cAiMesh()
{
    position = glm::vec3(0);
    orientation = glm::vec3(0);
    scale = glm::vec3(1);

    controller = nullptr;//new cAnimator(nullptr);
	importer = new Assimp::Importer();
    root = NULL;
}

cAiMesh::cAiMesh(const cAiMesh& copyIn)
{
    this->position = copyIn.position;
    this->orientation = copyIn.orientation;
    this->scale = copyIn.scale;

    controller = nullptr;//new cAnimator(nullptr);
    importer = nullptr;
    root = new aiScene(*copyIn.root);

    this->drawInfo = copyIn.drawInfo;
    this->isRelective = copyIn.isRelective;
    this->instanced = copyIn.instanced;
    this->nBones = copyIn.nBones;
    this->cubeMapTexNumber = copyIn.cubeMapTexNumber;
}

cAiMesh::~cAiMesh()
{
    if (controller)
    {
        delete controller;
    }

    for (int i = 0; i < animations.size(); i++)
    {
        delete animations[i];
    }

    if (importer)
    {
        delete importer;
        importer = nullptr;
    }
	
}

bool cAiMesh::LoadModel(std::string fileName)
{
	std::stringstream ss;
	ss << folderPath << fileName;

	root = importer->ReadFile(ss.str(), aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

    
	

	if (root->HasMeshes())
	{
		//load the meshes
		for (int i = 0; i < root->mNumMeshes; i++)
		{
			sAiModelDrawInfo info;
            
			aiMesh* temp = root->mMeshes[i];
            

			for (int j = 0; j < temp->mNumVertices; j++)
			{
                sVertex_XYZW_RGBA_N_UV_T_B vInfo;

                vInfo.bId1 = -1;
                vInfo.bId2 = -1;
                vInfo.bId3 = -1;
                vInfo.bId4 = -1;

                vInfo.bWeight1 = 0.0;
                vInfo.bWeight2 = 0.0;
                vInfo.bWeight3 = 0.0;
                vInfo.bWeight4 = 0.0;


                vInfo.x = temp->mVertices[j].x;
                vInfo.y = temp->mVertices[j].y;
                vInfo.z = temp->mVertices[j].z;
                vInfo.w = 1.0f;

                if (temp->HasNormals())
                {
                    vInfo.nx = temp->mNormals[j].x;
                    vInfo.ny = temp->mNormals[j].y;
                    vInfo.nz = temp->mNormals[j].z;
                    vInfo.nw = 1;
                }
                

                if (temp->mTextureCoords[0])
                {
                    vInfo.u0 = temp->mTextureCoords[0][j].x;
                    vInfo.v0 = temp->mTextureCoords[0][j].y;
                    vInfo.u1 = 0.0f;
                    vInfo.v1 = 0.0f;
                }
                
                if (temp->HasTangentsAndBitangents())
                {
                    vInfo.tx = temp->mTangents[j].x;
                    vInfo.ty = temp->mTangents[j].y;
                    vInfo.tz = temp->mTangents[j].z;
                    vInfo.tw = 1;

                    vInfo.bx = temp->mBitangents[j].x;
                    vInfo.by = temp->mBitangents[j].y;
                    vInfo.bz = temp->mBitangents[j].z;
                    vInfo.bw = 1;
                }

                vInfo.r = 1;
                vInfo.g = 1;
                vInfo.b = 1;
                vInfo.a = 1;
                

                
                if (temp->mMaterialIndex >= 0)
                {
                    aiMaterial* mat = root->mMaterials[temp->mMaterialIndex];
                    
                    aiColor3D colour;
                    mat->Get(AI_MATKEY_COLOR_DIFFUSE, colour);
                    vInfo.r = colour.r;
                    vInfo.g = colour.g;
                    vInfo.b = colour.b;

                    float shininess;
                    mat->Get(AI_MATKEY_SHININESS_STRENGTH, shininess);

                    info.specInfo.a = shininess;

                    mat->Get(AI_MATKEY_COLOR_SPECULAR, colour);
                    info.specInfo.r = colour.r;
                    info.specInfo.g = colour.g;
                    info.specInfo.b = colour.b;

                    std::string shiny;
                    mat->Get(AI_MATKEY_REFLECTIVITY, shiny);
                    int qwer = 9;
                }

                info.pVertices.push_back(vInfo);
			}

            

            
            info.numberOfVertices = temp->mNumVertices;
            info.numberOfIndices = temp->mNumFaces * 3;
            info.numberOfTriangles = temp->mNumFaces;

            info.pIndices.resize(info.numberOfIndices);

            unsigned int index = 0;
            for (int j = 0; j < temp->mNumFaces; j++)
            {
                aiFace tempFace = temp->mFaces[j];
                for (int k = 0; k < tempFace.mNumIndices; k++)
                {
                    info.pIndices[index + k] = tempFace.mIndices[k];
                }
                index += tempFace.mNumIndices;
            }


            

            info.VertexBuffer_Start_Index = 0;
            info.IndexBuffer_Start_Index = 0;
            drawInfo.push_back(info);


            //this is where you start
            //get a bone id per vertex, can be many ids
            //std::vector<aiBone*> boneNodes;
            if (temp->HasBones())
            {
                
                for (int j = 0; j < temp->mNumBones; j++)
                {
                    int boneID = -1;
                    std::string boneName = temp->mBones[j]->mName.C_Str();
                    if (this->bones.find(boneName) == this->bones.end())
                    {
                        BoneInfo newBoneInfo;
                        newBoneInfo.id = this->nBones;
                        newBoneInfo.offset = ai_to_glm(temp->mBones[j]->mOffsetMatrix);
                        this->bones[boneName] = newBoneInfo;
                        boneID = this->nBones;
                        this->nBones++;
                    }
                    else
                    {
                        boneID = this->bones[boneName].id;
                    }
                    assert(boneID != -1);
                    aiVertexWeight* weights = temp->mBones[j]->mWeights;
                    int numWeights = temp->mBones[j]->mNumWeights;

                    for (int weightIndex = 0; weightIndex < numWeights; weightIndex++)
                    {
                        int vertexId = weights[weightIndex].mVertexId;
                        float weight = weights[weightIndex].mWeight;
                        assert(vertexId <= drawInfo[i].pVertices.size());
                        drawInfo[i].AddBoneInfo(vertexId, boneID, weight);
                    }
                }
                
               
            }

		}
	}

    if (root->HasAnimations())
    {
        this->controller = new cAnimator(nullptr);
        //cAnimation(aiAnimation* animationNode, cAiMesh* model);
        for (int i = 0; i < root->mNumAnimations; i++)
        {
            cAnimation* newAnimation = new cAnimation(root->mAnimations[i], this);

            animations.push_back(newAnimation);
        }
    }
    

	if (root->HasTextures())
	{
        aiTexture* tempTex = root->mTextures[0];
        int asdffsdf = 98;
	}

	return false;
}

bool cAiMesh::LoadIntoVAO(GLuint shaderProgramID)
{
    for (int i = 0; i < drawInfo.size(); i++)
    {
        sAiModelDrawInfo& drawInfo = this->drawInfo[i];

        // Create a VAO (Vertex Array Object), which will 
        //	keep track of all the 'state' needed to draw 
        //	from this buffer...
        glUseProgram(shaderProgramID);


        // Ask OpenGL for a new buffer ID...
        glGenVertexArrays(1, &(drawInfo.VAO_ID));
        // "Bind" this buffer:
        // - aka "make this the 'current' VAO buffer
        glBindVertexArray(drawInfo.VAO_ID);

        // Now ANY state that is related to vertex or index buffer
        //	and vertex attribute layout, is stored in the 'state' 
        //	of the VAO... 


        // NOTE: OpenGL error checks have been omitted for brevity
    //	glGenBuffers(1, &vertex_buffer);
        glGenBuffers(1, &(drawInfo.VertexBufferID));

        //	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, drawInfo.VertexBufferID);
        // sVert vertices[3]


        glBufferData(GL_ARRAY_BUFFER,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B) * drawInfo.numberOfVertices,	// ::g_NumberOfVertsToDraw,	// sizeof(vertices), 
            (GLvoid*)drawInfo.pVertices.data(),							// pVertices,			//vertices, 
            GL_STATIC_DRAW);



        // Copy the index buffer into the video card, too
        // Create an index buffer.
        glGenBuffers(1, &(drawInfo.IndexBufferID));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawInfo.IndexBufferID);

        //unsigned int* copy = drawInfo.pIndices.data();

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,			// Type: Index element array
            sizeof(unsigned int) * drawInfo.numberOfIndices,
            (GLvoid*)drawInfo.pIndices.data(),
            GL_STATIC_DRAW);

        // ****************************************************************
        // Set the vertex attributes.


        //struct sVertex_XYZW_RGBA
        //{
        //    float x, y, z, w;   // Same as vec4 vPosition
        //    float r, g, b, a;   // Same as vec4 vColour
        //};

        // Set the vertex attributes for this shader
        GLint vpos_location = glGetAttribLocation(shaderProgramID, "vPosition");	    // program
        glEnableVertexAttribArray(vpos_location);	    // vPosition
        glVertexAttribPointer(vpos_location, 4,		// vPosition
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,      
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, x));

        GLint vcol_location = glGetAttribLocation(shaderProgramID, "vColour");	// program;
        glEnableVertexAttribArray(vcol_location);	    // vColour
        glVertexAttribPointer(vcol_location, 4,		// vColour
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),     // Stride // sizeof(float) * 6,   
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, r));

        // And all these, too:
        //in vec4 vNormal;				// Vertex normal X,Y,Z (W ignored)
        GLint vNormal_location = glGetAttribLocation(shaderProgramID, "vNormal");	// program;
        glEnableVertexAttribArray(vNormal_location);	    // vNormal
        glVertexAttribPointer(vNormal_location, 4,		    // vNormal
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, nx));


        //in vec4 vUVx2;					// 2 x Texture coords (vec4) UV0, UV1
        GLint vUVx2_location = glGetAttribLocation(shaderProgramID, "vUVx2");	// program;
        glEnableVertexAttribArray(vUVx2_location);	        // vUVx2
        glVertexAttribPointer(vUVx2_location, 4,		    // vUVx2
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, u0));


        //in vec4 vTangent;				// For bump mapping X,Y,Z (W ignored)
        GLint vTangent_location = glGetAttribLocation(shaderProgramID, "vTangent");	// program;
        glEnableVertexAttribArray(vTangent_location);	    // vTangent
        glVertexAttribPointer(vTangent_location, 4,		    // vTangent
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, tx));


        //in vec4 vBiNormal;				// For bump mapping X,Y,Z (W ignored)
        GLint vBiNormal_location = glGetAttribLocation(shaderProgramID, "vBiNormal");	// program;
        glEnableVertexAttribArray(vBiNormal_location);	        // vBiNormal
        glVertexAttribPointer(vBiNormal_location, 4,		    // vBiNormal
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bx));

        GLint vBoneId_location = glGetAttribLocation(shaderProgramID, "vBoneId");	// program;
        glEnableVertexAttribArray(vBoneId_location);	    
        glVertexAttribIPointer(vBoneId_location, 4,		    
            GL_INT,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bId1));

        GLint vBoneWeight_location = glGetAttribLocation(shaderProgramID, "vBoneWeight");	// program;
        glEnableVertexAttribArray(vBoneWeight_location);	    
        glVertexAttribPointer(vBoneWeight_location, 4,		    
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex_XYZW_RGBA_N_UV_T_B),
            (void*)offsetof(sVertex_XYZW_RGBA_N_UV_T_B, bWeight1));


        // ****************************************************************

        // Now that all the parts are set up, set the VAO to zero
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(vpos_location);
        //glDisableVertexAttribArray(vcol_location);
        // And the newer ones:
        glDisableVertexAttribArray(vNormal_location);	    // vNormal
        glDisableVertexAttribArray(vUVx2_location);	        // vUVx2
        glDisableVertexAttribArray(vTangent_location);	    // vTangent
        glDisableVertexAttribArray(vBiNormal_location);	        // vBiNormal
        glDisableVertexAttribArray(vBoneId_location);	       //bones
        glDisableVertexAttribArray(vBoneWeight_location);	       //bones

    }
	return true;
}

void cAiMesh::SetFolderPath(std::string path)
{
	folderPath = path;
}

void cAiMesh::UpdateMatricies()
{
    

    for (int i = 0; i < drawInfo.size(); i++)
    {
        //aiMesh* temp = root->mMeshes[i];
        drawInfo[i].boneMats = controller->finalBoneMatrices;
    }
}

cAnimation* cAiMesh::BlendAnimation(int animationNumber)
{
    cAnimation* curAnim = controller->currentAnimation;
    cAnimation* nextAnimation = animations[animationNumber];

    cAnimation* newAnim = new cAnimation(curAnim);
    float time = 2;
    for (int i = 0; i < curAnim->bones.size(); i++)
    {
        newAnim->bones[i].positions.clear();
        newAnim->bones[i].positions.resize(2);
        newAnim->bones[i].positions[0].position = curAnim->bones[i].localPosition;
        newAnim->bones[i].positions[0].timeStamp = 0;
    
        newAnim->bones[i].positions[1].position = nextAnimation->bones[i].positions[0].position;
        newAnim->bones[i].positions[1].timeStamp = time;

        newAnim->bones[i].rotations.clear();
        newAnim->bones[i].rotations.resize(2);
        newAnim->bones[i].rotations[0].orientation = curAnim->bones[i].localDirection;
        newAnim->bones[i].rotations[0].timeStamp = 0;

        newAnim->bones[i].rotations[1].orientation = nextAnimation->bones[i].rotations[0].orientation;
        newAnim->bones[i].rotations[1].timeStamp = time;

        newAnim->bones[i].scales.clear();
        newAnim->bones[i].scales.resize(2);
        newAnim->bones[i].scales[0].scale = curAnim->bones[i].localScale;
        newAnim->bones[i].scales[0].timeStamp = 0;
                          
        newAnim->bones[i].scales[1].scale = nextAnimation->bones[i].scales[0].scale;
        newAnim->bones[i].scales[1].timeStamp = time;

    }

    
    newAnim->ticksPerSecond = 24;
    newAnim->duration = time;

    this->controller->PlayAnimation(newAnim);
    this->controller->loop = true;
    
    return newAnim;
}

void cAiMesh::Update(float deltaTime)
{
    controller->UpdateAnimation(deltaTime);
    if (state)
    {
        state->Update(this, deltaTime);
    }
    //UpdateMatricies();
}

void cAiMesh::PlayAnimation(int animationNumber)
{
    if (animationNumber < animations.size())
    {
        controller->PlayAnimation(animations[animationNumber]);
    }
}

void cAiMesh::SetState(cMeshState* newState)
{
    if(state)
        state->Exit(this);

    delete state;

    state = newState;

    state->Enter(this);
}


void cAiMesh::Toggle()
{
    state->Toggle(this);
}

void cAiMesh::Render(GLuint shaderProgramID, glm::vec3 eye, glm::vec3 at)
{
    glUseProgram(shaderProgramID);

    if (textureNumber >= 0)
    {
        GLuint textureUnit = 12;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, this->textureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        //GLint texture_00_LocID = glGetUniformLocation(shaderProgramID, "texture_00");
        glUniform1i(300, textureUnit);

    }

    {
        GLuint textureUnit = 42;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeMapTexNumber);
        GLint cubeMap_01_LocID = glGetUniformLocation(shaderProgramID, "cubeMap");
        glUniform1i(cubeMap_01_LocID, textureUnit);
    }


    glm::mat4 matTranslate = glm::translate(glm::mat4(1.0f),
        position);

    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f),
        orientation.z,//(float)glfwGetTime(),
        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
        orientation.y,
        glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************

    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
        orientation.x,
        glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************


    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
        scale);
//matModel = matModel * matScale;
// *****************************************************
    glm::mat4 matModel(1);
// *****************************************************
    matModel = matModel * matTranslate;
    matModel = matModel * rotateZ;
    matModel = matModel * rotateY;
    matModel = matModel * rotateX;
    matModel = matModel * matScale;     // <-- mathematically, this is 1st

    // Now the matModel ("Model" or "World") matrix
    //  represents ALL the transformations we want, in ONE matrix.

    // Moved view and projection ("v" and "p") to outside draw scene loop.
    // (because they are the same for all objects)
    
    
    
    GLint matModel_Location = glGetUniformLocation(shaderProgramID, "matModel");

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));


    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));

    GLint matModelInverseTranspose_Location = glGetUniformLocation(shaderProgramID, "matModelInverseTranspose");
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));



    //view and stuff
    glm::mat4 matProjection = glm::perspective(
        .6f,
        16/9.0f,
        .1f,      // Near plane (as large as possible)
        1000.0f);      // Far plane (as small as possible)

    //matProjection = glm::perspective(
    //    0.6f,       // in degrees
    //    ratio,
    //    10.0f,       // Near plane (as large as possible)
    //    1'000'000.0f);   // Far plane (as small as possible)


    glm::vec3 cameraEye = eye;
    glm::vec3 cameraAt = at;
    glm::vec3 cameraUp = glm::vec3(0,1,0);


    glm::mat4 matView = glm::mat4(1.0f);
    matView = glm::lookAt(cameraEye,   // "eye"
        cameraAt,    // "at"
        cameraUp);


    GLint matView_loc = glGetUniformLocation(shaderProgramID,"matView");
    GLint matProj_loc = glGetUniformLocation(shaderProgramID, "matProjection");
    GLint matCam_loc = glGetUniformLocation(shaderProgramID, "cameraPosition");

    glUniformMatrix4fv(matView_loc, 1, GL_FALSE, glm::value_ptr(matView));
    glUniformMatrix4fv(matProj_loc, 1, GL_FALSE, glm::value_ptr(matProjection));
    //glUniform3f(matCam_loc, cameraEye.x, cameraEye.y, cameraEye.z);





    //get the number of bones
    GLint iNumberOfBones_loc = glGetUniformLocation(shaderProgramID, "NumberOfBones");
    glUniform1ui(iNumberOfBones_loc, (GLuint)this->nBones);

    GLint bReflect_loc = glGetUniformLocation(shaderProgramID, "bReflect");
    //GLint bRefract_loc = glGetUniformLocation(program, "bRefract");

    if (this->isRelective)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Override the colour...
        glUniform1f(bReflect_loc, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bReflect_loc, (float)GL_FALSE);
    }

    

    //upload the matricies
    for (int j = 0; j < this->nBones; j++)
    {
        std::stringstream ss;
        ss << "BoneMatrices[" << j << "]";

        GLint matrix_loc = glGetUniformLocation(shaderProgramID, ss.str().c_str());

        glm::mat4 tempMat = controller->finalBoneMatrices[j];
        glUniformMatrix4fv(matrix_loc, 1, GL_FALSE, glm::value_ptr(controller->finalBoneMatrices[j]));

    }
    
    for (int i = 0; i < drawInfo.size(); i++)
    {
        GLint specShine = glGetUniformLocation(shaderProgramID, "wholeObjectSpecularColour");
        glUniform4f(specShine, drawInfo[i].specInfo.r, drawInfo[i].specInfo.g, drawInfo[i].specInfo.b, drawInfo[i].specInfo.a);

        //glDisable(GL_BLEND);

        //glEnable(GL_DEPTH);

        glBindVertexArray(drawInfo[i].VAO_ID);

        if (instanced) 
        {
            glDrawElementsInstanced(GL_TRIANGLES,
                drawInfo[i].numberOfIndices,
                GL_UNSIGNED_INT,
                (void*)0,this->instanceCount); //draw MANY of them
        }
        else
        {
            glDrawElements(GL_TRIANGLES,
                drawInfo[i].numberOfIndices,
                GL_UNSIGNED_INT,
                (void*)0);
        }
        glBindVertexArray(0);
        //glDisable(GL_DEPTH);

        //glEnable(GL_BLEND);
    }

    glUniform1ui(iNumberOfBones_loc, (GLuint)0);
}


