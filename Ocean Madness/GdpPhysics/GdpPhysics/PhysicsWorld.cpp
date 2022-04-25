#include "PhysicsWorld.h"
#include <LinearMath/btVector3.h>
#include <LinearMath/btAlignedAllocator.h>
#include "../Conversion.h"



namespace gdpPhysics
{
	PhysicsWorld::PhysicsWorld()
	{
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		mBroadphase = new btAxisSweep3(worldMin, worldMax);

		mCollisionConfiguration = new btDefaultCollisionConfiguration();
		mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
		mSolver = new btSequentialImpulseConstraintSolver();

		mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
		
		debugRenderer.setDebugMode(btIDebugDraw::DBG_DrawWireframe + btIDebugDraw::DBG_DrawConstraints);
		//mDynamicsWorld->setDebugDrawer(&debugRenderer);
		
		mVersion = "Bullet";
		printf("Bullet Physics World\n");

	}

	PhysicsWorld::~PhysicsWorld()
	{
		// TODO:
		// Remove rigid bodies from the dynamics world
		// remove soft bodies from the soft rigid world
		// delete soft bodies
		// delete motion states
		// delete rigid bodies
		// Delete shapes.

		for (int i = 0; i < mBodies.size(); i++)
		{
			delete mBodies[i];
		}

		for (int i = 0; i < constraints.size(); i++)
		{
			delete constraints[i];
		}

		delete mDynamicsWorld;
		delete mSolver;
		delete mCollisionConfiguration;
		delete mDispatcher;
		delete mBroadphase;
	}

	void PhysicsWorld::SetGravity(const glm::vec3& gravity)
	{
		btVector3 btGravity;
		CastBulletVector3(gravity, &btGravity);

		mDynamicsWorld->setGravity(btGravity);
	}

	void PhysicsWorld::RegisterCollisionListener(iCollisionListener* listener)
	{
		// TODO:
	}

	void PhysicsWorld::AddConstraint(iCollisionBody* body1, iCollisionBody* body2)
	{
		btRigidBody* rb1 = CastBulletRigidBody(body1);
		btRigidBody* rb2 = CastBulletRigidBody(body2);



		btPoint2PointConstraint* constraint = new btPoint2PointConstraint(*rb1, *rb2, btVector3(1,0,0), btVector3(-3,0,0));

		rb1->addConstraintRef(constraint);
		rb2->addConstraintRef(constraint);

		constraints.push_back(constraint);
		mDynamicsWorld->addConstraint(constraint);
	}

	

	void PhysicsWorld::AddBody(iCollisionBody* body)
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


		mDynamicsWorld->addRigidBody(CastBulletRigidBody(body));
		
		
	}

	void PhysicsWorld::RemoveBody(iCollisionBody* body)
	{
		// TODO:
		btRigidBody* rb = CastBulletRigidBody(body);
		//if (rb->getConstraintRef(0))
		if (rb->getNumConstraintRefs() > 0)
		{
			mDynamicsWorld->removeConstraint(rb->getConstraintRef(0));
		}
		mDynamicsWorld->removeRigidBody(rb);
		
	}

	void PhysicsWorld::TimeStep(float dt)
	{
		for (int i = 0; i < mVehicles.size(); ++i)
		{
			mVehicles[i]->updateVehicle(dt);
		}


		mDynamicsWorld->stepSimulation(dt);
		mDynamicsWorld->debugDrawWorld();
		//mDynamicsWorld->debugDrawObject();
	}

	const std::string& PhysicsWorld::GetVersion()
	{
		return mVersion;
	}
}