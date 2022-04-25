#pragma once

#include <glm/glm.hpp>
#include "iRigidBody.h"
#include "iCollisionListener.h"
#include <string>
#include "../../iCollisionBody.h"

namespace gdpPhysics
{
	class iPhysicsWorld
	{
	public:
		virtual ~iPhysicsWorld() {}

		virtual void SetGravity(const glm::vec3& gravity) = 0;
		virtual void RegisterCollisionListener(iCollisionListener* listener) = 0;

		virtual void AddBody(iCollisionBody* body) = 0;
		virtual void RemoveBody(iCollisionBody* body) = 0;

		virtual void AddConstraint(iCollisionBody* body1, iCollisionBody* body2) = 0;

		virtual void TimeStep(float dt) = 0;

		virtual const std::string& GetVersion() = 0;

	protected:
		iPhysicsWorld() {}

	private:
		iPhysicsWorld(const iPhysicsWorld&) {}
		iPhysicsWorld& operator=(const iPhysicsWorld&) { return *this; }
	};
}
