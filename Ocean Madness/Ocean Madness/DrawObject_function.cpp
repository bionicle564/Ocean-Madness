#include "cMesh.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "GLCommon.h"       // Gives us glad.h
#include "cVAOManager.h"
#include "globalThings.h"


void GLFW_window_size_callback(GLFWwindow* window, int width, int height)
{
    // TODO: GLFW_window_size_callback()

    return;
}

// HACK: We shouldn't do the getUniformLocation every frame.
//  These don't change, so we should store them outside    
void SetUpTextures(cMesh* pCurrentMesh, GLuint shaderProgram)
{
    //*****************************************************************************************
    if ( pCurrentMesh->textureRatios[0] >= 0.0f )
    {
        // uniform sampler2D texture_00;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Fauci.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[0]);

        GLuint textureUnit = 0;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_00_LocID = glGetUniformLocation(shaderProgram, "texture_00");
        glUniform1i(texture_00_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if ( pCurrentMesh->textureRatios[1] >= 0.0f) 
    {
        // uniform sampler2D texture_01;
        //GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Pebbleswithquarzite.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[1]);

        GLuint textureUnit = 1;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_01_LocID = glGetUniformLocation(shaderProgram, "texture_01");
        glUniform1i(texture_01_LocID, textureUnit);
    }
    //*****************************************************************************************

    //*****************************************************************************************
    if ( pCurrentMesh->textureRatios[2] >= 0.0f) 
    {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[2]);

        GLuint textureUnit = 2;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_02_LocID = glGetUniformLocation(shaderProgram, "texture_02");
        glUniform1i(texture_02_LocID, textureUnit);
    }
    //*****************************************************************************************    

    //*****************************************************************************************
    if ( pCurrentMesh->textureRatios[3] >= 0.0f) {
        // uniform sampler2D texture_01;
//        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Broc_tree_house.bmp");
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->textureNames[3]);

        GLuint textureUnit = 3;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        // THIS SHOULDN'T BE HERE as it's the same each time and getUniformLocation is SLOOOOOOW
        GLint texture_03_LocID = glGetUniformLocation(shaderProgram, "texture_03");
        glUniform1i(texture_03_LocID, textureUnit);
    }
    //*****************************************************************************************    

    // Set all the texture ratios in the shader
    GLint textureRatios0to3_LocID = glGetUniformLocation(shaderProgram, "textureRatios0to3");
    // Set them
    glUniform4f( textureRatios0to3_LocID,
        pCurrentMesh->textureRatios[0], 
        pCurrentMesh->textureRatios[1], 
        pCurrentMesh->textureRatios[2],
        pCurrentMesh->textureRatios[3] );

    {
        GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("daySky");
        GLuint textureUnit = 41;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);
        GLint cubeMap_01_LocID = glGetUniformLocation(shaderProgram, "cubeMap");
        glUniform1i(cubeMap_01_LocID, textureUnit);
    }


    return;
}



void DrawObject(cMesh* pCurrentMesh, glm::mat4 matModel,
                GLint matModel_Location,
                GLint matModelInverseTranspose_Location,
                GLuint program,
                cVAOManager* pVAOManager)
{
    
    // Set up textures for this object
    SetUpTextures( pCurrentMesh, program );

    //{
    //    GLuint TextureNumber = ::g_pTextureManager->getTextureIDFromName("Space01");
    //    GLuint textureUnit = 41;			// Texture unit go from 0 to 79
    //    glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    //    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureNumber);
    //    GLint cubeMap_01_LocID = glGetUniformLocation(program, "cubeMap");
    //    glUniform1i(cubeMap_01_LocID, textureUnit);
    //}

    if (pCurrentMesh->textureRatios[2] == 1)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
    // Alpha transparency
    //glEnable(GL_BLEND);
    // Basic "alpha transparency"
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Get the uniform (should be outside of the draw call)
    GLint wholeObjectAlphaTransparency_LocID = glGetUniformLocation(program, "wholeObjectAlphaTransparency");
    // Set this value here
    glUniform1f(wholeObjectAlphaTransparency_LocID, pCurrentMesh->alphaTransparency);


    // *****************************************************
    // Translate or "move" the object somewhere
    glm::mat4 matTranslate = glm::translate( glm::mat4(1.0f),
                                                pCurrentMesh->positionXYZ );
    //matModel = matModel * matTranslate;
    // *****************************************************


    // *****************************************************
    // Rotation around the Z axis
    glm::mat4 rotateZ = glm::rotate( glm::mat4(1.0f),
                                        pCurrentMesh->orientationXYZ.z,//(float)glfwGetTime(),
                                        glm::vec3(0.0f, 0.0f, 1.0f));
    //matModel = matModel * rotateZ;
    // *****************************************************

    // *****************************************************
    // Rotation around the Y axis
    glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f),
                                    pCurrentMesh->orientationXYZ.y,
                                    glm::vec3(0.0f, 1.0f, 0.0f));
    //matModel = matModel * rotateY;
    // *****************************************************
            
    // *****************************************************
    // Rotation around the X axis
    glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f),
                                    pCurrentMesh->orientationXYZ.x,
                                    glm::vec3(1.0f, 0.0f, 0.0f));
    //matModel = matModel * rotateX;
    // *****************************************************

            
    // *****************************************************
    // Scale the model
    glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
                                    glm::vec3(pCurrentMesh->scale * pCurrentMesh->xScale,  // Scale in X
                                                pCurrentMesh->scale,  // Scale in Y
                                                pCurrentMesh->scale));// Scale in Z
    //matModel = matModel * matScale;
    // *****************************************************

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

    glUniformMatrix4fv(matModel_Location, 1, GL_FALSE, glm::value_ptr(matModel));


    // Inverse transpose of the model matrix
    // (Used to calculate the normal location in vertex space, using only rotation)
    glm::mat4 matInvTransposeModel = glm::inverse(glm::transpose(matModel));
    glUniformMatrix4fv(matModelInverseTranspose_Location, 1, GL_FALSE, glm::value_ptr(matInvTransposeModel));


    // Copy the whole object colour information to the sahder               

    // This is used for wireframe or whole object colour. 
    // If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
    GLint bUseDebugColour_Location = glGetUniformLocation(program, "bUseDebugColour");
    GLint objectDebugColour_Location = glGetUniformLocation(program, "objectDebugColour");

    // If true, then the lighting contribution is NOT used. 
    // This is useful for wireframe object
    GLint bDontLightObject_Location = glGetUniformLocation(program, "bDontLightObject");
    
    GLint bReflect_loc = glGetUniformLocation(program, "bReflect");
    GLint bRefract_loc = glGetUniformLocation(program, "bRefract");



    // The "whole object" colour (diffuse and specular)
    GLint wholeObjectDiffuseColour_Location = glGetUniformLocation(program, "wholeObjectDiffuseColour");
    GLint bUseWholeObjectDiffuseColour_Location = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
    GLint wholeObjectSpecularColour_Location = glGetUniformLocation(program, "wholeObjectSpecularColour");

    if (pCurrentMesh->bUseWholeObjectDiffuseColour)
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_TRUE);
        glUniform4f(wholeObjectDiffuseColour_Location, 
                    pCurrentMesh->wholeObjectDiffuseRGBA.r,
                    pCurrentMesh->wholeObjectDiffuseRGBA.g,
                    pCurrentMesh->wholeObjectDiffuseRGBA.b,
                    pCurrentMesh->wholeObjectDiffuseRGBA.a);
    }
    else
    {
        glUniform1f(bUseWholeObjectDiffuseColour_Location, (float)GL_FALSE);
    }

    glUniform4f(wholeObjectSpecularColour_Location, 
                pCurrentMesh->wholeObjectSpecularRGB.r,
                pCurrentMesh->wholeObjectSpecularRGB.g,
                pCurrentMesh->wholeObjectSpecularRGB.b,
                pCurrentMesh->wholeObjectShininess_SpecPower);


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bUseObjectDebugColour)
    {
        // Override the colour...
        glUniform1f(bUseDebugColour_Location, (float)GL_TRUE);
        glUniform4f(objectDebugColour_Location,
                    pCurrentMesh->objectDebugColourRGBA.r, 
                    pCurrentMesh->objectDebugColourRGBA.g,
                    pCurrentMesh->objectDebugColourRGBA.b,
                    pCurrentMesh->objectDebugColourRGBA.a);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bUseDebugColour_Location, (float)GL_FALSE);
    }


    // See if mesh is wanting the vertex colour override (HACK) to be used?
    if (pCurrentMesh->bDontLight)
    {
        // Override the colour...
        glUniform1f(bDontLightObject_Location, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bDontLightObject_Location, (float)GL_FALSE);
    }


    // Wireframe
    if (pCurrentMesh->bIsWireframe)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Draw everything with only lines
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    //reflect
    if (pCurrentMesh->reflect)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Override the colour...
        glUniform1f(bReflect_loc, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bReflect_loc, (float)GL_FALSE);
    }

    //refract
    if (pCurrentMesh->refract)                // GL_POINT, GL_LINE, and GL_FILL)
    {
        // Override the colour...
        glUniform1f(bRefract_loc, (float)GL_TRUE);
    }
    else
    {
        // DON'T override the colour
        glUniform1f(bRefract_loc, (float)GL_FALSE);
    }

            


    // glPointSize(20.0f); sometimes this works... Hey, it's not my fault!



    sModelDrawInfo modelInfo;
//        if (gVAOManager.FindDrawInfoByModelName("bun_zipper_res2 (justXYZ).ply", modelInfo))
//        if (gVAOManager.FindDrawInfoByModelName("Assembled_ISS.ply", modelInfo))

    if ( pVAOManager->FindDrawInfoByModelName( pCurrentMesh->meshName, modelInfo) )
    {
        glBindVertexArray(modelInfo.VAO_ID);

        glDrawElements(GL_TRIANGLES,
                        modelInfo.numberOfIndices, 
                        GL_UNSIGNED_INT, 
                        (void*)0);

        glBindVertexArray(0);
    }


    return;
}
