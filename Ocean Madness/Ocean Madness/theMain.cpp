
#include "GLCommon.h"
//#include <glad/glad.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include "globalThings.h"

//#include "linmath.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> 
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/gtx/rotate_vector.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>      // "string stream" just like iostream and fstream
#include <vector>       // "smart array"
#include <fstream>      // C++ file I-O library (look like any other stream)
#include <algorithm>



// Function signature for DrawObject()
#include "cAiMesh.h"
#include "cRunState.h"
#include "cIdleState.h"
#include "cBlendState.h"

#include "cBoat.h"


#include "cBoatWorld.h"

#include "cEnemyBoat.h"
#include "cEnemyManager.h"

#include "cWeather.h"

glm::vec3 AngleToDirection(glm::vec3 input);


void DrawObject(
    cMesh* pCurrentMesh, 
    glm::mat4 matModel,
    GLint matModel_Location,
    GLint matModelInverseTranspose_Location,
    GLuint program,
    cVAOManager* pVAOManager);




void DrawLoop(GLint* indexes, GLFWwindow* pWindow, GLuint program, float deltaTime, cFBO* fbo, glm::vec3 eyeIn, glm::vec3 atIn);

bool IsACloserToCamera(cMesh* pA, cMesh* pB)
{
    float distToCamera_A = glm::distance(pA->positionXYZ, ::g_pFlyCamera->getEye());
    float distToCamera_B = glm::distance(pB->positionXYZ, ::g_pFlyCamera->getEye());

    if (distToCamera_A > distToCamera_B)
    {
        return true;
    }
    return false;
}

cMesh* pSkyBox = new cMesh();




int main(int argc, char** argv)
{
    srand(time(NULL));
    GLFWwindow* pWindow;


    GLuint program = 0;     // 0 means "no shader program"

    GLint mvp_location = -1;        // Because glGetAttribLocation() returns -1 on error

    glfwSetErrorCallback(GLFW_error_callback);

    if ( ! glfwInit() )
    {
        return -1;
        //exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    pWindow = glfwCreateWindow(1280, 720, "Ocean Madness", NULL, NULL);

    if (!pWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pWindow, GLFW_key_callback);
    // NEW: 
    glfwSetCursorEnterCallback( pWindow, GLFW_cursor_enter_callback );
    glfwSetCursorPosCallback( pWindow, GLFW_cursor_position_callback );
    glfwSetScrollCallback( pWindow, GLFW_scroll_callback );
    glfwSetMouseButtonCallback( pWindow, GLFW_mouse_button_callback );
    glfwSetWindowSizeCallback( pWindow, GLFW_window_size_callback );

    glfwMakeContextCurrent(pWindow);
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);


    // void glGetIntegerv(GLenum pname, GLint * data);
    // See how many active uniform variables (registers) I can have
    GLint max_uniform_location = 0;
    GLint* p_max_uniform_location = NULL;
    p_max_uniform_location = &max_uniform_location;
    glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);


    std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

    // Create global things
    ::g_StartUp(pWindow);

    ::g_pFlyCamera->setEye( glm::vec3(0.1f, 0.8f, -15.0f) );


    cShaderManager::cShader vertShader;
    vertShader.fileName = "assets/shaders/vertShader_01.glsl";
        
    cShaderManager::cShader fragShader;
    fragShader.fileName = "assets/shaders/fragShader_01.glsl";

    if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, fragShader))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }


    GLuint waterProgram = -1;
    cShaderManager::cShader vertShader2;
    vertShader2.fileName = "assets/shaders/waterVert.glsl";

    cShaderManager::cShader fragShader2;
    fragShader2.fileName = "assets/shaders/waterFrag.glsl";

    if (::g_pShaderManager->createProgramFromFile("WaterShader", vertShader2, fragShader2))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        waterProgram = ::g_pShaderManager->getIDFromFriendlyName("WaterShader");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }

    GLuint simpleProgram = -1;
    cShaderManager::cShader vertShader3;
    vertShader3.fileName = "assets/shaders/simpleVert.glsl";

    cShaderManager::cShader fragShader3;
    fragShader3.fileName = "assets/shaders/simpleFrag.glsl";

    if (::g_pShaderManager->createProgramFromFile("simpleShader", vertShader3, fragShader3))
    {
        std::cout << "Shader compiled OK" << std::endl;
        // 
        // Set the "program" variable to the one the Shader Manager used...
        simpleProgram = ::g_pShaderManager->getIDFromFriendlyName("simpleShader");
    }
    else
    {
        std::cout << "Error making shader program: " << std::endl;
        std::cout << ::g_pShaderManager->getLastError() << std::endl;
    }


    g_program = program;
    // Select the shader program we want to use
    // (Note we only have one shader program at this point)
    glUseProgram(program);


    //GLint mvp_location = -1;
    mvp_location = glGetUniformLocation(program, "MVP");

    // Get "uniform locations" (aka the registers these are in)
    GLint matModel_Location = glGetUniformLocation(program, "matModel");
    GLint matView_Location = glGetUniformLocation(program, "matView");
    GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
    GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");
    GLint eyeLocation_Location = glGetUniformLocation(program, "eyeLocation");

    GLint* indexes = new GLint[6];
    indexes[1] = matView_Location;
    indexes[2] = matProjection_Location;
    indexes[3] = eyeLocation_Location;
    indexes[4] = matModel_Location;
    indexes[5] = matModelInverseTranspose_Location;

    cLightManager secondShaderLights;

    // Get the uniform locations of the light shader values
    ::g_pTheLights->SetUpUniformLocations(program);
    //::g_pTheLights->SetUpUniformLocations(waterProgram);
    
    glUseProgram(waterProgram);
    secondShaderLights.SetUpUniformLocations(waterProgram);

    glUseProgram(program);

    // Set up the debug sphere object
    ::g_pDebugSphere = new cMesh();
    ::g_pDebugSphere->meshName = "ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply";
    ::g_pDebugSphere->bIsWireframe = true;
    ::g_pDebugSphere->bUseObjectDebugColour = true;
    ::g_pDebugSphere->objectDebugColourRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    ::g_pDebugSphere->bDontLight = true;
    ::g_pDebugSphere->scale = 1.0f;
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // Give this a friendly name
    ::g_pDebugSphere->friendlyName = "Debug Sphere";
    // Then I could make a small function that searches for that name


    


    ::g_pVAOManager->setFilePath("assets/models/");

    std::vector<std::string> vecModelsToLoad;
    vecModelsToLoad.push_back("ISO_Shphere_flat_3div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply");
    vecModelsToLoad.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
    //vecModelsToLoad.push_back("cube.ply");
    //vecModelsToLoad.push_back("testthing.ply");

    unsigned int totalVerticesLoaded = 0;
    unsigned int totalTrianglesLoaded = 0;
    for (std::vector<std::string>::iterator itModel = vecModelsToLoad.begin(); itModel != vecModelsToLoad.end(); itModel++)
    {
        sModelDrawInfo theModel;
        std::string modelName = *itModel;
        std::cout << "Loading " << modelName << "...";
        if ( !::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program) )
        {
            std::cout << "didn't work because: " << std::endl;
            std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
        }
        else
        {
            std::cout << "OK." << std::endl;
            std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
            totalTrianglesLoaded += theModel.numberOfTriangles;
            totalVerticesLoaded += theModel.numberOfVertices;
        }


    }//for (std::vector<std::string>::iterator itModel


    std::cout << "Done loading models." << std::endl;
    std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
    std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;



    
    //this loads scene meshes from a txt

    //g_vec_pMeshes = loader->LoadMeshFile();
    //for (int i = 0; i < g_vec_pMeshes.size(); i++)
    //{
    //    cMesh* curPtr = g_vec_pMeshes[i];
    //    sModelDrawInfo tempInfo;
    //    if (!g_pVAOManager->FindDrawInfoByModelName(curPtr->meshName, tempInfo))
    //    {
    //        if (!g_pVAOManager->LoadModelIntoVAO(curPtr->meshName, tempInfo, g_program))
    //        {
    //            std::cout << "Didn't load ply file" << std::endl;
    //        }
    //    }
    //}

    sModelDrawInfo tempInfo;
    g_pVAOManager->LoadModelIntoVAO("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply", tempInfo, g_program);



    g_pTheLights->LoadLightInformationFromFile("lights.txt");
    
    secondShaderLights.LoadLightInformationFromFile("lights.txt");


    loader->LoadTextureNames();

    //This is from Fenny
    std::string errorTextString;
    ::g_pTextureManager->SetBasePath("assets/textures");
    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
        "SpaceBox_right1_posX.bmp",     /* posX_fileName */
        "SpaceBox_left2_negX.bmp",      /*negX_fileName */
        "SpaceBox_top3_posY.bmp",       /*posY_fileName*/
        "SpaceBox_bottom4_negY.bmp",    /*negY_fileName*/
        "SpaceBox_front5_posZ.bmp",     /*posZ_fileName*/
        "SpaceBox_back6_negZ.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the space cube texture OK" << std::endl;
    }

    if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("daySky",
        "TropicalSunnyDayRight2048.bmp",     /* posX_fileName */
        "TropicalSunnyDayLeft2048.bmp",      /*negX_fileName */
        "TropicalSunnyDayDown2048.bmp",       /*posY_fileName*/
        "TropicalSunnyDayUp2048.bmp",    /*negY_fileName*/
        "TropicalSunnyDayBack2048.bmp",     /*posZ_fileName*/
        "TropicalSunnyDayFront2048.bmp",      /*negZ_fileName*/
        true, errorTextString))
    {
        std::cout << "Didn't load because: " << errorTextString << std::endl;
    }
    else
    {
        std::cout << "Loaded the space cube texture OK" << std::endl;
    }


    // Create a skybox object (a sphere with inverted normals that moves with the camera eye)
    
    pSkyBox->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
    
    pSkyBox->scale = ::g_pFlyCamera->nearPlane * 1000.0f;
    
    pSkyBox->positionXYZ = ::g_pFlyCamera->getEye();

    

    //g_vec_pMeshes.push_back(box);

    secondShaderLights.theLights[0].param1.x = 2;
    //g_pTheLights->theLights[0].atten.x = 1;
    //g_pTheLights->theLights[0].atten.y = .09;
    //g_pTheLights->theLights[0].atten.z = .032;

    //secondShaderLights.theLights[0].param2.y = .3;
    //g_pTheLights->theLights[0].param1.y = glm::radians(10.0f);
    //g_pTheLights->theLights[0].param1.z = glm::radians(30.0f);
    secondShaderLights.theLights[0].direction = glm::vec4(.5,-1,0,0);
    //g_pTheLights->TurnOnLight(0);

    //g_pTheLights->theLights[1].param1.x = 0;
    //secondShaderLights.TurnOffLight(0);

    //g_pTheLights->CopyLightInfoToShader();

    //g_pTheLights->CopyLightInfoToShader();

    const double MAX_DELTA_TIME = 0.1;  // 100 ms
    double previousTime = glfwGetTime();

    gFBO = new cFBO();

    std::string errorString;
    if (gFBO->init(1920, 1080, errorString))
    {
        std::cout << "FBO good to go" << std::endl;
    }
    else
    {
        std::cout << "FBO error: " << errorString << std::endl;
    }

    //gFBO->clearBuffers();

    cFBO* screenFBO = new cFBO();
    if (screenFBO->init(1920, 1080, errorString))
    {
        std::cout << "FBO good to go" << std::endl;
    }
    else
    {
        std::cout << "FBO error: " << errorString << std::endl;
    }


    float timer = 0;
    float endTime = .05f;

    GLuint passNumber_loc = glGetUniformLocation(program, "passNumber");
    indexes[0] = passNumber_loc;



    cAiMesh playerMesh;
    playerMesh.SetFolderPath("assets\\models\\");
    playerMesh.LoadModel("fixed_knight.fbx");
    playerMesh.LoadIntoVAO(program);
    playerMesh.SetState(new cIdleState());
    playerMesh.scale = glm::vec3(.01f);
    playerMesh.position.x = -5;
    playerMesh.orientation.y = 3.14f;
    

    cAiMesh enemyShip;
    enemyShip.SetFolderPath("assets\\models\\");
    enemyShip.LoadModel("ship.fbx");
    enemyShip.LoadIntoVAO(program);
    enemyShip.scale = glm::vec3(.01f);
    enemyShip.position.z = 10;



    GLuint ids[3];
    ids[0] = program;
    ids[1] = waterProgram;
    ids[2] = g_pTextureManager->getTextureIDFromName("toon_water.bmp");

    cBoat boat(nullptr, ids);


    cAiMesh testMesh2;
    testMesh2.SetFolderPath("assets\\models\\");
    testMesh2.LoadModel("cannon_ball.fbx");
    testMesh2.LoadIntoVAO(program);
    testMesh2.position.y = 6;
    testMesh2.position.x = 0;

    cAiMesh testMesh3;
    testMesh3.SetFolderPath("assets\\models\\");
    testMesh3.LoadModel("rain.fbx");
    testMesh3.LoadIntoVAO(program);
    testMesh3.position.y = 15;
    testMesh3.position.x = 0;
    testMesh3.orientation.x = glm::radians(-90.0f);
    testMesh3.isRelective = true;
    testMesh3.cubeMapTexNumber = ::g_pTextureManager->getTextureIDFromName("daySky");


    cAiMesh tempCopy = cAiMesh(testMesh2);


    cBoatWorld world;
    world.Init(boat.worldPosition, boat.boat.orientation);
    world.cannonBall = testMesh2;
    //world.meshList = &boat.projectileMeshes;

    //game states and vars
    bool playerIsIdle = true;

    int side = 1;
    cAiMesh* selectedCannon = &boat.starboardCannon;
    bool aimMode = false;
    glm::vec3 aimVec;
    glm::vec3 aimEye;

    const float MAX_SHOT_TIME = 2.0f;
    float shotTimer = MAX_SHOT_TIME;

    float boatSpeed = 1.0f;

    bool sailMode = false;


    cEnemyManager enemies(program);


    double deltaTime;


    enemies.projectiles = &world.projectiles;
    
    enemies.SetUpBoats(10);


    //cWeather weatherSystem(program, 10);

    //weatherSystem.Init();
    
    world.playerPosition = ((world.GetPlayerPosition())-boat.boat.position);

    glEnable(GL_DEPTH_TEST);

    gFBO->clearBuffers();
    float sinTimer = 0;
    while (!glfwWindowShouldClose(pWindow))
    {
        g_TitleText = "Ocean Madness";

        //std::cout << position.y << "\n";
        
        gFBO->clearBuffers();
        float ratio = 16/9.0f;
        int width, height;
        glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
        glm::mat4 matProjection;        // used to be "p";
        glm::mat4 matView;              // used to be "v";

        double currentTime = glfwGetTime();
//        double deltaTime = previousTime - currentTime;        OOOPSS!
        deltaTime = currentTime - previousTime;
        deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
        previousTime = currentTime;


        enemies.deltaTime = deltaTime;
        

        sinTimer += (float)deltaTime * 2.f;

        sinTimer = fmod(sinTimer, 2*3.14f);

        shotTimer -= deltaTime;
        
        glm::vec3 direction(0);
        direction.z = -sin(boat.boat.orientation.y);
        direction.x = cos(boat.boat.orientation.y);
        direction *= deltaTime;

        boat.worldPosition += direction * boatSpeed;

        //world.player->ApplyForce(direction);
        //world.player->ApplyImpulse(direction);
        //boat.worldPosition += glm::vec3(1 * deltaTime, 0, 0);
        
        
        world.boatPosition = boat.worldPosition;
        world.boatOrientation = boat.boat.orientation;

        glm::vec3 position = world.GetBoatPosition();
        //ground->GetPosition(position);

        boat.boat.position = position;

        //capsule->GetPosition(position);
        position = world.GetPlayerPosition();
        world.playerPosition = ((position) - boat.boat.position);

        //playerMesh.position = world.playerPosition + boat.boat.position + glm::vec3(0, -1.5, 0);
        playerMesh.position = world.playerPosition + boat.boat.position + glm::vec3(0, -1.5, 0);
        //playerMesh.orientation.y = world.boatOrientation.y;
        


        if (sailMode)
        {
            glm::vec3 cameraOffset = glm::rotateY(glm::vec3(-8, 400, 0), world.boatOrientation.y);
            g_pFlyCamera->setEye(playerMesh.position + cameraOffset);
            g_pFlyCamera->hardcodedAt = playerMesh.position; // g_pFlyCamera->getAt(); //
        }
        else if (!aimMode)
        {

            glm::vec3 cameraOffset = glm::rotateY(glm::vec3(-8, 20, 0), world.boatOrientation.y);
            g_pFlyCamera->setEye(playerMesh.position + cameraOffset);
            g_pFlyCamera->hardcodedAt = playerMesh.position; // g_pFlyCamera->getAt(); // 
        }
        else
        {
            aimEye = selectedCannon->position + glm::vec3(0, 1, 0);
            aimVec = AngleToDirection(selectedCannon->orientation);
            aimVec *= side;
            //aimVec += +glm::vec3(0, 0, 1);
            g_pFlyCamera->setEye(aimEye);
            g_pFlyCamera->hardcodedAt = g_pFlyCamera->getEye() + glm::normalize(aimVec); // g_pFlyCamera->getAt(); //
        }

        //update other threads
        enemies.camaeraAt = g_pFlyCamera->hardcodedAt;
        enemies.camaeraEye = g_pFlyCamera->getEye();

        boat.Update((float)deltaTime);

        //glfwGetFramebufferSize(pWindow, &width, &height);
        //ratio = width / (float)height;
        glm::vec3 cameraEye = ::g_pFlyCamera->getEye(); //testMesh.position + glm::vec3(0, 10, 0);
        //=======================================================

        timer += (float)deltaTime;
        if (timer >= endTime)
        {
            float rng =(float)( rand() % 50 / 100.0);
            
            GLint rng_loc = glGetUniformLocation(program, "rand");
            glUniform1f(rng_loc, rng);
            timer = 0;
        }

        //testMesh1.orientation.y = glfwGetTime();
        //if (switchAnimation)
        //{
        //    playerMesh.PlayAnimation(animationIndex);
        //    switchAnimation = false;
        //}

        if (space)
        {
            if (aimMode)
            {
                aimMode = false;
            }
            else
            {




                if (glm::distance(playerMesh.position, boat.starboardCannon.position) < 2)
                {
                    selectedCannon = &boat.starboardCannon;
                    side = -1;
                    //world.ShootCannonBall(glm::vec3(0, 6, 0), glm::vec3(0,150, 150));
                    //std::cout << "Cannon Ball!\n";
                    aimEye = selectedCannon->position + glm::vec3(0, 1, 0);
                    aimVec = glm::vec3(0, 0, 1);
                    aimMode = !aimMode;
                    shotTimer = 0;
                }
                else if (glm::distance(playerMesh.position, boat.portCannon.position) < 2)
                {
                    selectedCannon = &boat.portCannon;
                    side = 1;
                    //world.ShootCannonBall(glm::vec3(0, 6, 0), glm::vec3(0,150, 150));
                    //std::cout << "Cannon Ball!\n";
                    aimEye = selectedCannon->position + glm::vec3(0, 1, 0);
                    aimVec = glm::vec3(0, 0, -1);
                    aimMode = !aimMode;
                    shotTimer = 0;
                }
                else if (glm::distance(playerMesh.position, boat.wheel.position) < 2)
                {
                    sailMode = !sailMode;
                }
                else if (glm::distance(playerMesh.position, boat.wheel.position) > 2)
                {
                    sailMode = false;
                }

            }

            space = false;
        }

        if (xKey)
        {
            if (aimMode && shotTimer < 0)
            {
                shotTimer = MAX_SHOT_TIME;
                aimVec *= 6;
                glm::vec3 power = aimVec;
                power *= 100;
                world.ShootCannonBall(selectedCannon->position + aimVec, power);
                //world.ShootChainShot(selectedCannon->position + aimVec, power);
               
            }
            xKey = false;
            
        }

        if (zKey)
        {
            if (aimMode && shotTimer < 0)
            {
                shotTimer = MAX_SHOT_TIME + 2;
                aimVec *= 6;
                glm::vec3 power = aimVec;
                power *= 100;
                //world.ShootCannonBall(selectedCannon->position + aimVec, power);
                world.ShootChainShot(selectedCannon->position + aimVec, power);

            }
            zKey = false;

        }

        //capsule->CancelForces();
        world.player->CancelForces();

        float playerSpeed = 5;

    

        if (left)
        {
            if (!aimMode && !sailMode)
            {
                glm::vec3 direction = playerMesh.position - g_pFlyCamera->getEye();
                direction = glm::normalize(direction);
                direction *= playerSpeed * 2.0f;
                direction.y = 0;

                direction = glm::cross(direction, glm::vec3(0, 1, 0));
                
                world.player->ApplyImpulse(-direction);
                playerMesh.orientation.y = 3.14f + world.boatOrientation.y;
            }
            else if (sailMode)
            {
                boat.boat.orientation.y += deltaTime;

                glm::vec3 newOffset =  glm::rotateY(world.playerPosition, (float)deltaTime);
                world.player->SetOrigin(world.boatPosition + newOffset, world.boatOrientation);
            }
            else if(aimMode)
            {
                //if (selectedCannon->orientation.y < 1)
                if (selectedCannon->helperValue < 1)
                {
                    //aimVec.x += deltaTime;
                    //selectedCannon->orientation.y += deltaTime;
                    selectedCannon->helperValue += deltaTime;
                }
            }
        }

        if (right)
        {
            if (!aimMode && !sailMode)
            {
                glm::vec3 direction = playerMesh.position - g_pFlyCamera->getEye();
                direction = glm::normalize(direction);
                direction *= playerSpeed * 2.0f;
                direction.y = 0;

                direction = glm::cross(direction, glm::vec3(0, 1, 0));

                world.player->ApplyImpulse(direction);
                playerMesh.orientation.y = 0 + world.boatOrientation.y;
            }
            else if (sailMode)
            {
                boat.boat.orientation.y -= deltaTime;

                glm::vec3 newOffset = glm::rotateY(world.playerPosition, -(float)deltaTime);
                world.player->SetOrigin(world.boatPosition + newOffset, world.playerOrientation);
            }
            else if (aimMode)
            {
                //if (selectedCannon->orientation.y > -1)
                if (selectedCannon->helperValue > -1)
                {
                    selectedCannon->helperValue -= deltaTime;
                }
            }
        }
        

        //add force to match the speed of the boat, unless turning in sailmode
        //(don't ask why)
        if (sailMode)
        {
            boat.ocean.position = boat.boat.position + glm::vec3(-300, 0, 300);
            if (left || right)
            {
                world.player->ApplyImpulse(-direction * 70.f);

            }
        }
        world.player->ApplyImpulse(direction * 70.f);


        if (up)
        {
            if (!aimMode && !sailMode)
            {
                glm::vec3 direction =playerMesh.position - g_pFlyCamera->getEye();
                direction = glm::normalize(direction);
                direction *= playerSpeed * 2.0f;
                direction.y = 0;
                
                world.player->ApplyImpulse(direction);
                playerMesh.orientation.y = 1.5708f + world.boatOrientation.y;
            }
            else if (sailMode)
            {
                boatSpeed = 10;
            }
            else if (aimMode)
            {
                if (selectedCannon->orientation.x > -.5)
                {
                    selectedCannon->orientation.x -= deltaTime;
                }
            }
        }
        else
        {
            boatSpeed = 1;
        }

        
        if (down)
        {
            if (!aimMode && !sailMode)
            {
                glm::vec3 direction = playerMesh.position - g_pFlyCamera->getEye();
                direction = glm::normalize(direction);
                direction *= playerSpeed * 2.0f;
                direction.y = 0;

                world.player->ApplyImpulse(-direction);
                playerMesh.orientation.y = -1.5708f + world.boatOrientation.y;
            }
            else if (aimMode)
            {
                if (selectedCannon->orientation.x < .5)
                {
                    //aimVec.x += deltaTime;
                    selectedCannon->orientation.x += deltaTime;
                }
            }
        }

        if (up && right)
        {
            playerMesh.orientation.y = glm::radians(45.0f) + world.boatOrientation.y;
        }
        else if (up && left)
        {
            playerMesh.orientation.y = glm::radians(125.f) + world.boatOrientation.y;
        }
        else if (down && left)
        {
            playerMesh.orientation.y = glm::radians(225.f) + world.boatOrientation.y;
        }
        else if (down && right)
        {
            playerMesh.orientation.y = glm::radians(315.f) + world.boatOrientation.y;
        }

        if (!aimMode)
        {

            if (!up && !down && !left && !right && !playerIsIdle)
            {
                //box->CancelForces();
                playerIsIdle = true;
                playerMesh.SetState(new cBlendState(&playerMesh, 2, new cIdleState()));
            }
            else if ((up || down || left || right) && playerIsIdle)
            {
                playerMesh.SetState(new cBlendState(&playerMesh, 9, new cRunState()));
                playerIsIdle = false;
            }
            else
            {
                //playerIsIdle = false;
            }

        }

        

        g_pTheLights->theLights[0].position = glm::vec4(boat.boat.position + glm::vec3(0, 5, 0), 1);

        playerMesh.Update((float)deltaTime);

        
        //playerMesh.Update(deltaTime);
        //testMesh2.Update(deltaTime);

        for (int i = 0; i < ::g_pTheLights->NUMBER_OF_LIGHTS; i++)
        {
            secondShaderLights.theLights[i].position = ::g_pTheLights->theLights[i].position;
        }



        secondShaderLights.CopyLightInfoToShader(waterProgram);
        glUseProgram(program);

        GLuint TextureNumber = screenFBO->colourTexture_0_ID;

        GLuint textureUnit = 8;			// Texture unit go from 0 to 79
        glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
        glBindTexture(GL_TEXTURE_2D, TextureNumber);

        GLint texture_00_LocID = glGetUniformLocation(program, "texture_06");
        glUniform1i(texture_00_LocID, textureUnit);

        //DrawLoop(indexes, pWindow, program, deltaTime, screenFBO, screen->positionXYZ, glm::vec3(0, 0, -1));
        //DrawLoop(indexes, pWindow, program, deltaTime, screenFBO, screen->positionXYZ, screen->positionXYZ + glm::vec3(0,0,-1));
        
        //DrawLoop(indexes, pWindow, program, (float)deltaTime, gFBO, cameraEye, g_pFlyCamera->getAt());
        DrawLoop(indexes, pWindow, program, (float)deltaTime, gFBO, cameraEye, g_pFlyCamera->GetHardAt());
        
        
        //weatherSystem.Update(deltaTime, cameraEye, g_pFlyCamera->GetHardAt());
        
        playerMesh.Render(program, cameraEye, g_pFlyCamera->GetHardAt());
        //testMesh3.Render(program, cameraEye, g_pFlyCamera->GetHardAt());

        enemies.Update(deltaTime, enemies.camaeraEye, enemies.camaeraAt);
        //glBindFramebuffer(GL_FRAMEBUFFER, gFBO->ID);

        glUseProgram(waterProgram);
        glUniform1f(6, sinTimer);
        //testMesh4.Render(waterProgram, cameraEye, g_pFlyCamera->getAt());

        //enemyShip.Render(program, cameraEye, g_pFlyCamera->GetHardAt());
        //enemy.Update(deltaTime, program, cameraEye, g_pFlyCamera->GetHardAt());

        boat.Render(ids, cameraEye, g_pFlyCamera->GetHardAt());

        glUseProgram(program);

        //Start of second pass
        if (fullScreenQuad == NULL)
        {
            fullScreenQuad = new cMesh();
            fullScreenQuad->friendlyName = "screenQuad";
            fullScreenQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
            fullScreenQuad->scale = 100;
            fullScreenQuad->positionXYZ = glm::vec3(0, 0, 500);
            fullScreenQuad->bDontLight = true;
            fullScreenQuad->orientationXYZ.y = 3.14f;

            GLuint TextureNumber = gFBO->colourTexture_0_ID;
           
            GLuint textureUnit = 7;			// Texture unit go from 0 to 79
            glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_2D, TextureNumber);

            GLint texture_00_LocID = glGetUniformLocation(program, "texture_07");
            glUniform1i(texture_00_LocID, textureUnit);


            GLuint TextureNumber2 = gFBO->depthTexture_ID;

            textureUnit = 20;			// Texture unit go from 0 to 79
            glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_2D, TextureNumber2);

            texture_00_LocID = glGetUniformLocation(program, "depthMap");
            glUniform1i(texture_00_LocID, textureUnit);

          
            GLuint TextureNumber3 = gFBO->vertexMaterialColour_1_ID;
            textureUnit = 22;			// Texture unit go from 0 to 79
            glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
            glBindTexture(GL_TEXTURE_2D, TextureNumber3);

            texture_00_LocID = glGetUniformLocation(program, "normalMap");
            glUniform1i(texture_00_LocID, textureUnit);

            glBindTexture(GL_TEXTURE_2D, 0);


        }
        glfwGetFramebufferSize(pWindow, &width, &height);
        ratio = width / (float)height;

        matProjection = glm::perspective(
            ::g_pFlyCamera->FOV,
            ratio,
            ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
            ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)


        ::g_pFlyCamera->Update(deltaTime);

        cameraEye = g_pFlyCamera->getEye();
        glm::vec3 cameraAt = g_pFlyCamera->GetHardAt();
        glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


       matView = glm::mat4(1.0f);
        matView = glm::lookAt(cameraEye,   // "eye"
            cameraAt,    // "at"
            cameraUp);

        glUseProgram(simpleProgram);
        GLint matViewLoc = glGetUniformLocation(simpleProgram, "matView");
        GLint matProjLoc = glGetUniformLocation(simpleProgram, "matProjection");
        glUniformMatrix4fv(matViewLoc, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(matProjLoc, 1, GL_FALSE, glm::value_ptr(matProjection));

        glEnable(GL_DEPTH_TEST);
        //world->TimeStep(deltaTime);
        world.Update((float)deltaTime, program, cameraEye, cameraAt);
        world.player->ApplyImpulse(direction);


        glDisable(GL_DEPTH_TEST);

        glUseProgram(program);
        glUniform1ui(passNumber_loc, 1);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glClearColor(1, 99 / 255.0f, 71 / 255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //glViewport(0, 0, ::gFBO->width, ::gFBO->height);
        //ratio = ::gFBO->width / (float)::gFBO->height;

        

        glViewport(0, 0, width, height);

        GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
        glUniform2f(screenWidthHeight_locID, (float)width, (float)height);

        matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
        glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
        glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
        glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
        matView = glm::lookAt(eyeForFullScreenQuad,
            atForFullScreenQuad,
            upForFullScreenQuad);      // up in y direction

//detail::tmat4x4<T> glm::gtc::matrix_transform::ortho	(	T const & 	left,
//                                                         T const & 	right,
//                                                         T const & 	bottom,
//                                                         T const & 	top,
//                                                         T const & 	zNear,
//                                                         T const & 	zFar )		
        matProjection = glm::ortho(
            0.0f,   // Left
            1.0f / (float)width,  // Right
            0.0f,   // Top
            1.0f / (float)height, // Bottom
            30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
            70.0f); // zFar



        //cameraEye = glm::vec3(0, 0, -100000);//g_pFlyCamera->getEye(); 

        //matView = glm::mat4(1.0f);
        //matView = glm::lookAt(cameraEye,   // "eye"
        //    glm::vec3(0,0,1),    // "at" g_pFlyCamera->getAt()
        //    cameraUp);


        //matProjection = glm::perspective(
        //    ::g_pFlyCamera->FOV,
        //    ratio,
        //    ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
        //    ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

        

        

        glUniformMatrix4fv(matView_Location, 1, GL_FALSE, glm::value_ptr(matView));
        glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));
        glUniform4f(eyeLocation_Location, cameraEye.x, cameraEye.y, cameraEye.z, 0);

        glCullFace(GL_FRONT);
        //glCullFace(GL_BACK);


        // All the draw code was here:
        DrawObject(fullScreenQuad, matModel, matModel_Location, matModelInverseTranspose_Location, program, g_pVAOManager);
        //DrawDebugObjects(matModel_Location, matModelInverseTranspose_Location, program, ::g_pVAOManager);


        // "Present" what we've drawn.
        glfwSwapBuffers(pWindow);        // Show what we've drawn

        // Process any events that have happened
        glfwPollEvents();

        // Handle OUR keyboard, mouse stuff
        handleAsyncKeyboard(pWindow, deltaTime);
        handleAsyncMouse(pWindow, deltaTime);

    }//while (!glfwWindowShouldClose(window))

    //delete ground; //for now
    //delete capsule;


    delete fullScreenQuad;
    delete gFBO;
    delete screenFBO;
    // All done, so delete things...
    ::g_ShutDown(pWindow);


    glfwDestroyWindow(pWindow);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

glm::vec3 AngleToDirection(glm::vec3 input)
{
    glm::vec3 output;

    //get components from trig functions
    output.y = sin(input.x);
    output.x = cos(input.x) * sin(input.y);
    output.z = cos(input.x) * cos(input.y);

    //invert the x and z axes
    output.z *= -1;
    output.x *= -1;

    return output;
}

void DrawLoop(GLint* indexes, GLFWwindow* pWindow,GLuint program, float deltaTime, cFBO* fbo, glm::vec3 eyeIn, glm::vec3 atIn)
{
    glm::mat4 matModel;

    glBindFramebuffer(GL_FRAMEBUFFER, fbo->ID);

    glUniform1ui(indexes[0], 0);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, fbo->width, fbo->height);
    float ratio = fbo->width / (float)fbo->height;

    


    // Turn on the depth buffer
    glEnable(GL_DEPTH);         // Turns on the depth buffer
    glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

    fbo->clearBuffers(true, true);


    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // *******************************************************
    // Screen is cleared and we are ready to draw the scene...
    // *******************************************************

    // Update the title text
    glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());


    // Copy the light information into the shader to draw the scene
    ::g_pTheLights->CopyLightInfoToShader(program);

    // Place the "debug sphere" at the same location as the selected light (again)
    ::g_pDebugSphere->positionXYZ = ::g_pTheLights->theLights[0].position;
    // HACK: Debug sphere is 5th item added
//        ::g_vecMeshes[5].positionXYZ = gTheLights.theLights[0].position;

    glm::mat4 matProjection = glm::perspective(
        ::g_pFlyCamera->FOV,
        ratio,
        ::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
        ::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

    //matProjection = glm::perspective(
    //    0.6f,       // in degrees
    //    ratio,
    //    10.0f,       // Near plane (as large as possible)
    //    1'000'000.0f);   // Far plane (as small as possible)

    ::g_pFlyCamera->Update(deltaTime);

    glm::vec3 cameraEye = eyeIn;
    glm::vec3 cameraAt = atIn;
    glm::vec3 cameraUp = ::g_pFlyCamera->getUpVector();


    glm::mat4 matView = glm::mat4(1.0f);
    matView = glm::lookAt(cameraEye,   // "eye"
        cameraAt,    // "at"
        cameraUp);


    glUniformMatrix4fv(indexes[1], 1, GL_FALSE, glm::value_ptr(matView));
    glUniformMatrix4fv(indexes[2], 1, GL_FALSE, glm::value_ptr(matProjection));
    glUniform4f(indexes[3], cameraEye.x, cameraEye.y, cameraEye.z, 0);



    GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
    glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);


    glDisable(GL_DEPTH_TEST);
    // Move the "skybox object" with the camera
    pSkyBox->positionXYZ = ::g_pFlyCamera->getEye();
    DrawObject(
        pSkyBox, glm::mat4(1.0f),
        indexes[4], indexes[5],
        program, ::g_pVAOManager);

    glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
    //
    glEnable(GL_DEPTH_TEST);

    

    GLint camera_loc = glGetUniformLocation(program, "cameraPosition");
    glm::vec3 eye = g_pFlyCamera->getEye();
    glUniform3f(camera_loc, eye.x, eye.y, eye.z);






    // **********************************************************************
    // Draw the "scene" of all objects.
    // i.e. go through the vector and draw each one...
    // **********************************************************************

    std::vector< cMesh* > vecTransparentObjects;
    std::vector< cMesh* > vecOpaqueObjects;

    for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
    {
        // So the code is a little easier...
        cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

        if (pCurrentMesh->alphaTransparency < 1.0f || pCurrentMesh->textureRatios[2] == 1)
        {
            vecTransparentObjects.push_back(pCurrentMesh);
        }
        else
        {
            vecOpaqueObjects.push_back(pCurrentMesh);
        }
    }


    std::sort(vecTransparentObjects.begin(), vecTransparentObjects.end(), IsACloserToCamera);

    GLint bIsFbo_loc = glGetUniformLocation(program, "bIsFbo");

    // Draw the opaque things first 
    glDisable(GL_BLEND);
    for (std::vector< cMesh* >::iterator it_pCurMesh = vecOpaqueObjects.begin();
        it_pCurMesh != vecOpaqueObjects.end(); it_pCurMesh++)
    {
        cMesh* pTheObject = *it_pCurMesh;
        if (pTheObject->bIsFBO)
        {
            glUniform1f(bIsFbo_loc, (GLfloat)GL_TRUE);
        }
        else
        {
            glUniform1f(bIsFbo_loc, (GLfloat)GL_FALSE);
        }

        matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

        // All the draw code was here:
        DrawObject(pTheObject, matModel, indexes[4], indexes[5], program, g_pVAOManager);

    }//for ( std::vector< cMeshObject* >




    // Draw the see-through things next 
    glEnable(GL_BLEND);
    for (std::vector< cMesh* >::iterator it_pCurMesh = vecTransparentObjects.begin();
        it_pCurMesh != vecTransparentObjects.end(); it_pCurMesh++)
    {
        cMesh* pTheObject = *it_pCurMesh;
        if (pTheObject->bIsFBO)
        {
            glUniform1f(bIsFbo_loc, (GLfloat)GL_TRUE);
        }
        else
        {
            glUniform1f(bIsFbo_loc, (GLfloat)GL_FALSE);
        }
        matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)

        // All the draw code was here:
        DrawObject(pTheObject, matModel, indexes[4], indexes[5], program, g_pVAOManager);

    }//for ( std::vector< cMeshObject* >
}