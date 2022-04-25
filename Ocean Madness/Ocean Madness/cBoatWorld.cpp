#include "cBoatWorld.h"
#include <gdpPhysics/interfaces/shapes.h>
#include "../GdpPhysics/GdpPhysics/PhysicsFactory.h" //the one referance to GdpPhysics
#include <glm/gtx/rotate_vector.hpp>

cBoatWorld::cBoatWorld()
{
    


	//this->world = world;
    portRailOffset = glm::vec3(0,3,-4);
    starboardRailOffset = glm::vec3(0, 3, 4);
    frontWallOffset = glm::vec3(10, 3, 0);
    backWallOffset = glm::vec3(-10, 3, 0);

    wheelWallOffset = glm::vec3(-6,3,0);


	factory = new gdpPhysics::PhysicsFactory();
	world = factory->CreateWorld();
}

cBoatWorld::~cBoatWorld()
{
    for (int i = 0; i < projectiles.size(); i++)
    {
        delete projectiles[i].body;
    }

	delete factory;
	delete world;

    delete portRail;
    delete starboardRail;
    delete frontWall;

    delete wheelWall;

    delete boat;
    delete player;
}

void cBoatWorld::Init(glm::vec3 boatPosition, glm::vec3 boatOrientaion)
{
    gdpPhysics::iShape* boxShape = new gdpPhysics::BoxShape(glm::vec3(10, 2.5, 4));
    gdpPhysics::cRigidBodyDesc desc;
    desc.linearDamping = 0.001f;
    desc.angularDamping = 0.001f;
    desc.isStatic = false;
    desc.mass = 0;
    desc.position = boatPosition;
    desc.linearVelocity = glm::vec3(0.f);
    desc.restitution = 0.0f;
    desc.friction = 1;
    desc.rotation = glm::quat(boatOrientaion);
    boat = factory->CreateRigidBody(desc, boxShape);

    gdpPhysics::iShape* boxShape2 = new gdpPhysics::CapsuleShape(glm::vec3(.6, 2.2, 1));
    gdpPhysics::cRigidBodyDesc desc2;
    desc2.linearDamping = 0.001f;
    desc2.angularDamping = 0.001f;
    desc2.isStatic = true;
    desc2.mass = 1;
    desc2.position = glm::vec3(0, 4.5, 0);
    desc2.linearVelocity = glm::vec3(0.f);
    desc2.restitution = 0.0f;
    desc2.rotation = glm::vec3(0);
    player = factory->CreateRigidBody(desc2, boxShape2);
    playerPosition = desc2.position;

    gdpPhysics::iShape* boxShape3 = new gdpPhysics::BoxShape(glm::vec3(10, .5, .5));
    gdpPhysics::cRigidBodyDesc desc3;
    desc3.linearDamping = 0.001f;
    desc3.angularDamping = 0.001f;
    desc3.isStatic = false;
    desc3.mass = 0;
    desc3.position = boatPosition + portRailOffset;
    desc3.linearVelocity = glm::vec3(0.f);
    desc3.restitution = 0.0f;
    desc3.rotation = glm::vec3(0);
    portRail = factory->CreateRigidBody(desc3, boxShape3);

    gdpPhysics::iShape* boxShape4 = new gdpPhysics::BoxShape(glm::vec3(10, .5, .5));
    gdpPhysics::cRigidBodyDesc desc4;
    desc4.linearDamping = 0.001f;
    desc4.angularDamping = 0.001f;
    desc4.isStatic = false;
    desc4.mass = 0;
    desc4.position = boatPosition + starboardRailOffset;
    desc4.linearVelocity = glm::vec3(0.f);
    desc4.restitution = 0.0f;
    desc4.rotation = glm::vec3(0);
    starboardRail = factory->CreateRigidBody(desc4, boxShape4);

    gdpPhysics::iShape* boxShape5 = new gdpPhysics::BoxShape(glm::vec3(.5, .5, 5));
    gdpPhysics::cRigidBodyDesc desc5;
    desc5.linearDamping = 0.001f;
    desc5.angularDamping = 0.001f;
    desc5.isStatic = false;
    desc5.mass = 0;
    desc5.position = boatPosition + frontWallOffset;
    desc5.linearVelocity = glm::vec3(0.f);
    desc5.restitution = 0.0f;
    desc5.rotation = glm::vec3(0);
    frontWall = factory->CreateRigidBody(desc5, boxShape5);


    gdpPhysics::iShape* boxShape6 = new gdpPhysics::BoxShape(glm::vec3(.5, .5, 5));
    gdpPhysics::cRigidBodyDesc desc6;
    desc6.linearDamping = 0.001f;
    desc6.angularDamping = 0.001f;
    desc6.isStatic = false;
    desc6.mass = 0;
    desc6.position = boatPosition + backWallOffset;
    desc6.linearVelocity = glm::vec3(0.f);
    desc6.restitution = 0.0f;
    desc6.rotation = glm::vec3(0);
    backWall = factory->CreateRigidBody(desc6, boxShape6);


    gdpPhysics::iShape* boxShape7 = new gdpPhysics::BoxShape(glm::vec3(.2, 1, 1));
    gdpPhysics::cRigidBodyDesc desc7;
    desc7.linearDamping = 0.001f;
    desc7.angularDamping = 0.001f;
    desc7.isStatic = false;
    desc7.mass = 0;
    desc7.position = boatPosition + wheelWallOffset;
    desc7.linearVelocity = glm::vec3(0.f);
    desc7.restitution = 0.0f;
    desc7.rotation = glm::vec3(0);
    wheelWall = factory->CreateRigidBody(desc7, boxShape7);

    


    world->AddBody(boat);
    world->AddBody(player);
    world->AddBody(portRail);
    world->AddBody(starboardRail);
    world->AddBody(frontWall);
    world->AddBody(backWall);
    world->AddBody(wheelWall);



    world->SetGravity(glm::vec3(0, -9.8, 0));
}

void cBoatWorld::Update(float dt, GLuint program, glm::vec3 eye, glm::vec3 at)
{
    for (int i = 0; i < projectiles.size(); i++)
    {

        glm::vec3 position;
        projectiles[i].body->GetPosition(position);
        projectiles[i].mesh.position = position;
        projectiles[i].mesh.Render(program, eye, at);
        
        
        if (position.y < -4)
        {
            world->RemoveBody(projectiles[i].body);
            
            
            gdpPhysics::iRigidBody* pBody = projectiles[i].body;
            projectiles.erase(projectiles.begin() + i);
            //meshList.at(i).position.y = -6;
            //meshList->erase(meshList->begin() + i);
            //delete pBody;
            i--;
        }
        
    }

    glm::vec3 newOffset = portRailOffset;
    


    boat->SetOrigin(boatPosition, boatOrientation);


    //newOffset = glm::rotateY(playerPosition, boatOrientation.y);
    //player->SetOrigin(boatPosition + playerPosition, playerOrientation);

    newOffset = glm::rotateY(portRailOffset, boatOrientation.y);
    portRail->SetOrigin(boatPosition + newOffset, boatOrientation);
    
    newOffset = glm::rotateY(starboardRailOffset, boatOrientation.y);
    starboardRail->SetOrigin(boatPosition + newOffset, boatOrientation);

    newOffset = glm::rotateY(frontWallOffset, boatOrientation.y);
    frontWall->SetOrigin(boatPosition + newOffset, boatOrientation);


    newOffset = glm::rotateY(backWallOffset, boatOrientation.y);
    backWall->SetOrigin(boatPosition + newOffset, boatOrientation);

    newOffset = glm::rotateY(wheelWallOffset, boatOrientation.y);
    wheelWall->SetOrigin(boatPosition + newOffset, boatOrientation);

	world->TimeStep(dt);
}

glm::vec3 cBoatWorld::GetBoatPosition()
{
    glm::vec3 rtn;
    boat->GetPosition(rtn);
    return rtn;
}

void cBoatWorld::ShootCannonBall(glm::vec3 from, glm::vec3 direction)
{
    gdpPhysics::iShape* sphereShape = new gdpPhysics::SphereShape(1);
    gdpPhysics::cRigidBodyDesc desc;
    desc.linearDamping = 0.001f;
    desc.angularDamping = 0.001f;
    desc.isStatic = false;
    desc.mass = 10;
    //desc.mass = 5;
    desc.position = from;
    desc.linearVelocity = glm::vec3(0.f);
    desc.restitution = 0.0f;
    desc.friction = .3;
    //desc.rotation = glm::quat();
    gdpPhysics::iRigidBody* cannonBall = nullptr;
    while (cannonBall == nullptr)
    {
        cannonBall = factory->CreateRigidBody(desc, sphereShape);
        if (cannonBall == nullptr)
        {
            int asdf = 9;
        }
    }

    //cAiMesh newBall = cAiMesh(this->cannonBall);

    sCannonBall newCannonBall;
    newCannonBall.mesh = cAiMesh(this->cannonBall);
    newCannonBall.body = cannonBall;

    projectiles.push_back(newCannonBall);
    world->AddBody(newCannonBall.body);
    newCannonBall.body->ApplyImpulse(direction);

    
    //meshList.push_back(cAiMesh(this->cannonBall));
}

void cBoatWorld::ShootChainShot(glm::vec3 from, glm::vec3 direction)
{
    gdpPhysics::iShape* sphereShape = new gdpPhysics::SphereShape(1);
    gdpPhysics::cRigidBodyDesc desc;
    desc.linearDamping = 0.001f;
    desc.angularDamping = 0.001f;
    desc.isStatic = false;
    desc.mass = 10;
    //desc.mass = 5;
    desc.position = from + glm::vec3(-2.5,0,0);
    desc.linearVelocity = glm::vec3(0.f);
    desc.restitution = 0.0f;
    desc.friction = .3;
    //desc.rotation = glm::quat();
    gdpPhysics::iRigidBody* cannonBall1 = factory->CreateRigidBody(desc, sphereShape);

    sCannonBall newCannonBall;
    newCannonBall.mesh = cAiMesh(this->cannonBall);
    newCannonBall.body = cannonBall1;

    projectiles.push_back(newCannonBall);

    desc.position.x += 5;

    gdpPhysics::iRigidBody* cannonBall2 = factory->CreateRigidBody(desc, sphereShape);
    
    sCannonBall newCannonBall2;
    newCannonBall2.mesh = cAiMesh(this->cannonBall);
    newCannonBall2.body = cannonBall2;

    projectiles.push_back(newCannonBall2);

    cannonBall1->ApplyImpulse(direction);
    cannonBall2->ApplyImpulse(direction + glm::vec3(0,0,-100));
    //    

    ////cAiMesh newBall = cAiMesh(this->cannonBall);

    //sCannonBall newCannonBall;
    //newCannonBall.mesh = cAiMesh(this->cannonBall);
    //newCannonBall.body = cannonBall;

    //projectiles.push_back(newCannonBall);
    //world->AddBody(newCannonBall.body);
    //newCannonBall.body->ApplyImpulse(direction);

    world->AddBody(cannonBall1);
    world->AddBody(cannonBall2);

    world->AddConstraint(cannonBall1, cannonBall2);

}

glm::vec3 cBoatWorld::GetPlayerPosition()
{
    glm::vec3 rtn;
    player->GetPosition(rtn);
    return rtn;
    //return playerPosition;
}
