#pragma once

#include <gdpPhysics/interfaces/iPhysicsWorld.h>
#include <vector>

#include "RigidBody.h"
#include "CollisionHandler.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <iCollisionBody.h>

#include "../cDebugDrawer.h"

namespace gdpPhysics
{
	class PhysicsWorld : public iPhysicsWorld
	{
	public:
		PhysicsWorld();
		virtual ~PhysicsWorld();

		virtual void TimeStep(float dt);

		virtual void SetGravity(const glm::vec3& gravity);
		virtual void RegisterCollisionListener(iCollisionListener* listener);

		virtual void AddConstraint(iCollisionBody* body1, iCollisionBody* body2);

		virtual void AddBody(iCollisionBody* body);
		virtual void RemoveBody(iCollisionBody* body);

		virtual const std::string& GetVersion();

	private:
		cDebugDrawer debugRenderer;

		btBroadphaseInterface* mBroadphase;
		btDefaultCollisionConfiguration* mCollisionConfiguration;
		btCollisionDispatcher* mDispatcher;
		btSequentialImpulseConstraintSolver* mSolver;

		btDiscreteDynamicsWorld* mDynamicsWorld;

		std::vector<iCollisionBody*> mBodies;
		std::vector<btRaycastVehicle*> mVehicles;

		std::string mVersion;

		std::vector< btPoint2PointConstraint*> constraints;

		PhysicsWorld(const PhysicsWorld&) {}
		PhysicsWorld& operator=(const PhysicsWorld&) { return *this; }
	};
}