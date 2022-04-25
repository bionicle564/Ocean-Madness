#pragma once

#include <gdpPhysics/interfaces/iRigidBody.h>
#include <gdpPhysics/interfaces/cRigidBodyDesc.h>
#include <gdpPhysics/interfaces/iShape.h>

#include "CollisionHandler.h"
#include <btBulletDynamicsCommon.h>

namespace gdpPhysics
{
	class RigidBody : public iRigidBody
	{
	public:
		RigidBody(const cRigidBodyDesc& desc, iShape* shape);
		virtual ~RigidBody();

		iShape* GetShape();

		bool IsStatic() const;

		virtual void GetPosition(glm::vec3& positionOut);
		virtual void SetPosition(const glm::vec3& positionIn);

		virtual void GetOrientation(glm::quat& orientationOut);
		virtual void SetOrientation(const glm::quat& orientationIn);

		virtual void ApplyForce(const glm::vec3& force);
		virtual void ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint);

		virtual void ApplyImpulse(const glm::vec3& impulse);
		virtual void ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint);

		virtual void ApplyTorque(const glm::vec3& torque);
		virtual void ApplyTorqueImpulse(const glm::vec3& torqueImpulse);

		virtual void CancelForces();

		virtual void SetOrigin(glm::vec3 position, glm::vec3 orientation);

		btRigidBody* GetBulletBody() { return mBulletBody; }
	private:
		btRigidBody* mBulletBody;

		RigidBody(const RigidBody&) {}
		RigidBody& operator=(const RigidBody&) { return *this; }
	};
}