#include "globalThings.h"

#include <sstream>
#include <iostream>


// Turns off the: warning C4005: 'APIENTRY': macro redefinition
#pragma warning( disable: 4005)
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
void UseMessageBox(void)
{

    // If you want to do some Win32 calls:
    // Note the "L" which is "wide" or Unicode encoding
    //MessageBox(NULL, L"Moved Camera", L"FYI:", MB_OK);
    int response = MessageBox(NULL, L"Moved Camera", L"FYI:", MB_YESNOCANCEL | MB_ICONWARNING);
    switch (response)
    {
    case IDYES:
        std::cout << "You chose YES" << std::endl;
        break;
    case IDNO:
        std::cout << "You chose NO" << std::endl;
        break;
    case IDCANCEL:
        std::cout << "You chose CANCEL" << std::endl;
        break;
    }

    return;
}

void GLFW_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (g_selectedObject > 0)
        {
            g_selectedObject--;
        }
        else
        {
            g_selectedObject = g_vec_pMeshes.size() - 1;
        }
    }

    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
        g_selectedObject++;
        if (g_selectedObject == g_vec_pMeshes.size())
        {
            g_selectedObject = 0;
        }
    }

    if (key == GLFW_KEY_K && action == GLFW_PRESS)
    {
        loader->SaveMeshToFile(g_vec_pMeshes);
        g_pTheLights->SaveLightInformationToFile("lights.txt");

    }
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
    {
        g_vec_pMeshes = loader->LoadMeshFile();
        for (int i = 0; i < g_vec_pMeshes.size(); i++)
        {
            cMesh* curPtr = g_vec_pMeshes[i];
            sModelDrawInfo tempInfo;
            if (!g_pVAOManager->FindDrawInfoByModelName(curPtr->meshName, tempInfo))
            {
                if (!g_pVAOManager->LoadModelIntoVAO(curPtr->meshName, tempInfo, g_program))
                {
                    std::cout << "Didn't load ply file" << std::endl;
                }
            }
        }

        g_pTheLights->LoadLightInformationFromFile("lights.txt");
        loader->LoadTextureNames();

    }


    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        xKey = true;
    }
    else if (key == GLFW_KEY_X && action == GLFW_RELEASE)
    {
        xKey = false;
    }


    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        zKey = true;
    }
    else if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
    {
        zKey = false;
    }

    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cKey = true;
    }
    else if (key == GLFW_KEY_C && action == GLFW_RELEASE)
    {
        cKey = false;
    }


    if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        up = true;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
    {
        up = false;
    }


    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        down = true;
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
    {
        down = false;
    }

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        right = true;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
    {
        right = false;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        left = true;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
    {
        left = false;
    }

    //debug lighting (no lighting)
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        space = true;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        space = false;
    }

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        animationIndex++;
        if (animationIndex > 5)
        {
            animationIndex = 0;
        }
        switchAnimation = true;
    }
    
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        animationIndex--;
        if (animationIndex < 0)
        {
            animationIndex = 5;
        }
        switchAnimation = true;
    }
    

    

    //the idea was to take screen cords and get the colour of the pixel
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        double x;
        double y;
        glfwGetCursorPos(window, &x, &y);

        if ((x > 0 && x < 100) && (y > 0 && y< 100))
        {
            for (int i = 0; i < g_vec_pMeshes.size(); i++)
            {
                g_vec_pMeshes[i]->bDontLight = !g_vec_pMeshes[i]->bDontLight;
            }
        }

    }

    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        g_pTheLights->theLights[2].param2.x = !g_pTheLights->theLights[2].param2.x;
    }

    float cameraSpeed = 1.0f;
    float objectMovementSpeed = 1.0f;
    float lightMovementSpeed = 1.0f;

    bool bShiftDown = false;
    bool bControlDown = false;
    bool bAltDown = false;


    if ((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT)
    {
        // Shift is down and maybe other things, too
        bShiftDown = true;
    }
    if ((mods & GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)
    {
        // Shift is down and maybe other things, too
        bControlDown = true;
    }
    if ((mods & GLFW_MOD_ALT) == GLFW_MOD_ALT)
    {
        // Shift is down and maybe other things, too
        bAltDown = true;
    }

    if ((bShiftDown) && (!bControlDown) && !bAltDown)
    {
        if (key == GLFW_KEY_B && action == GLFW_PRESS)
        {
            g_vec_pMeshes[g_selectedObject]->bDontLight = !g_vec_pMeshes[g_selectedObject]->bDontLight;
        }
    }
 

   // If JUST the ALT is down, move the "selected" light
   if ( (!bShiftDown) && (!bControlDown) && bAltDown )
   {
        if ( key == GLFW_KEY_PAGE_UP )
        {
            ::g_bShowDebugShere = true;
        }
        if ( key == GLFW_KEY_PAGE_DOWN )
        {
            ::g_bShowDebugShere = false;
        }

        if ( key == GLFW_KEY_KP_ADD && action == GLFW_PRESS )
        {
            ::g_selectedLight++;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = cLightManager::NUMBER_OF_LIGHTS - 1;
            }
        }
        if ( key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS )
        {
            ::g_selectedLight--;
            if ( ::g_selectedLight >= cLightManager::NUMBER_OF_LIGHTS )
            {
                ::g_selectedLight = 0;
            }
        }


        if ( key == GLFW_KEY_KP_MULTIPLY )
        {
            // Turn light on
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 1.0f;
        }
        if ( key == GLFW_KEY_KP_DIVIDE )
        {
            // Turn light off
            ::g_pTheLights->theLights[::g_selectedLight].param2.x = 0.0f;
        }



        std::stringstream strTitle;
        // std::cout << 
        strTitle << "Light # " << ::g_selectedLight << " positionXYZ : "
            << ::g_pTheLights->theLights[::g_selectedLight].position.x << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.y << ", "
            << ::g_pTheLights->theLights[::g_selectedLight].position.z << "  "
            << "attenuation (C, L, Q): "
            << ::g_pTheLights->theLights[::g_selectedLight].atten.x << ", "        // Const
            << ::g_pTheLights->theLights[::g_selectedLight].atten.y << ", "        // Linear
            << ::g_pTheLights->theLights[::g_selectedLight].atten.z << "  "        // Quadratic
            << (::g_pTheLights->theLights[::g_selectedLight].param2.x > 0.0f ? " is on" : " is off");
        //<< std::endl;
        ::g_TitleText = strTitle.str();

 

    }//if ( bShiftDown && ( ! bControlDown ) && ( ! bAltDown ) )

    return;
}
