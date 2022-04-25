#include "cCollisionWorld.h"
#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedAllocator.h>
#include "Conversion.h"



namespace gdpPhysics
{
	cCollisionWorld::cCollisionWorld()
	{
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		mBroadphase = new btAxisSweep3(worldMin, worldMax);

		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mSolver = new btSequentialImpulseConstraintSolver();

		collisionWorld = new btCollisionWorld(mDispatcher, mBroadphase, mCollisionConfiguration);

		debugRenderer.setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints);
		collisionWorld->setDebugDrawer(&debugRenderer);

		mVersion = "Bullet";
		printf("Bullet Physics World\n");

	}

	cCollisionWorld::~cCollisionWorld()
	{
		// TODO:
		// Remove rigid bodies from the dynamics world
		// remove soft bodies from the soft rigid world
		// delete soft bodies
		// delete motion states
		// delete rigid bodies
		// Delete shapes.


		delete collisionWorld;
		delete mSolver;
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mBroadphase;
	}

	void cCollisionWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);

		//collisionWorld->setGravity(btGravity);
	}

	void cCollisionWorld::RegisterCollisionListener(iCollisionListener* listener)
	{
		// TODO:
	}



	void cCollisionWorld::AddBody(iCollisionBody* body)
	{
		if (std::find(mBodies.begin(), mBodies.end(), body) != mBodies.end())
			return;

		if (body->GetBodyType() == gdpPhysics::eBodyType::SOFT_BODY)
		{
			printf("PhysicsWorld.AddBody: [Error] Unable to add a soft body to a dynamics world!\n");
			return;
		}

		// TODO:
		// May want to add 2 more vectors
		// vector<SoftBody*> mSoftBodies;
		// vector<RigidBody*> mRigidBodies;
		mBodies.push_back(body);

		collisionWorld->addCollisionObject(nullptr);
		//collisionWorld->addRigidBody(CastBulletRigidBody(body));

	}

	void cCollisionWorld::RemoveBody(iCollisionBody* body)
	{
		// TODO:
	}

	void cCollisionWorld::TimeStep(float dt)
	{
		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->updateVehicle(dt);
		}

		//collisionWorld->contactTest()
		//collisionWorld->stepSimulation(dt);
		collisionWorld->debugDrawWorld();
		//mDynamicsWorld->debugDrawObject();
	}

	const std::string& cCollisionWorld::GetVersion()
	{
		return mVersion;
	}
}