#include "RigidBody.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "../Conversion.h"


namespace gdpPhysics
{
	RigidBody::RigidBody(const cRigidBodyDesc& desc, iShape* shape)
		: iRigidBody()
	{
		btQuaternion ori;
		btVector3 pos;

		CastBulletQuaternion(desc.rotation, &ori);
		CastBulletVector3(desc.position, &pos);

		btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(ori, pos));
		btVector3 inertia(0, 0, 0);
		btCollisionShape* bulletShape = CastBulletShape(shape);
		
		if (desc.mass != 0.f)
		{
			bulletShape->calculateLocalInertia(desc.mass, inertia);
		}

		btRigidBody::btRigidBodyConstructionInfo bodyCI(desc.mass, motionState, bulletShape, inertia);
		mBulletBody = new btRigidBody(bodyCI);

		btScalar rest;
		CastBulletScalar(desc.restitution, &rest);
		mBulletBody->setRestitution(rest);

		if(desc.isStatic)
		{
			mBulletBody->setSleepingThresholds(0.0, 0.0);
			mBulletBody->setAngularFactor(0.0);
		}

		mBulletBody->setMassProps(desc.mass, inertia);

		mBulletBody->setFriction(desc.friction);

		mBulletBody->setActivationState(ACTIVE_TAG);
	}

	RigidBody::~RigidBody() {}

	bool RigidBody::IsStatic() const
	{
		return mBulletBody->isStaticObject();
	}

	iShape* RigidBody::GetShape()
	{
		return 0;
	}

	void RigidBody::GetPosition(glm::vec3& positionOut)
	{
		CastGLMVector3(mBulletBody->getCenterOfMassPosition(), &positionOut);
	}

	void RigidBody::SetPosition(const glm::vec3& positionIn)
	{
		assert(0);
	}

	void RigidBody::GetOrientation(glm::quat& orientationOut)
	{
		CastGLMQuaternion(mBulletBody->getOrientation(), &orientationOut);
	}

	void RigidBody::SetOrientation(const glm::quat& orientationIn)
	{
		assert(0);
	}

	void RigidBody::ApplyForce(const glm::vec3& force)
	{
		btVector3 btForce;
		CastBulletVector3(force, &btForce);
		mBulletBody->setActivationState(ACTIVE_TAG);
		mBulletBody->applyCentralForce(btForce);
	}

	void RigidBody::ApplyForceAtPoint(const glm::vec3& force, const glm::vec3& relativePoint)
	{
		btVector3 btForce;
		btVector3 btForceAtPoint;

		CastBulletVector3(force, &btForce);
		CastBulletVector3(relativePoint, &btForceAtPoint);

		mBulletBody->applyForce(btForce, btForceAtPoint);
	}

	void RigidBody::ApplyImpulse(const glm::vec3& impulse)
	{
		btVector3 btImpulse;
		CastBulletVector3(impulse, &btImpulse);
		mBulletBody->setActivationState(ACTIVE_TAG);
		mBulletBody->applyCentralImpulse(btImpulse);
	}

	void RigidBody::ApplyImpulseAtPoint(const glm::vec3& impulse, const glm::vec3& relativePoint)
	{
		btVector3 btImpulse;
		btVector3 btImpulseAtPoint;

		CastBulletVector3(impulse, &btImpulse);
		CastBulletVector3(relativePoint, &btImpulseAtPoint);

		mBulletBody->applyImpulse(btImpulse, btImpulseAtPoint);
	}

	void RigidBody::ApplyTorque(const glm::vec3& torque)
	{
		btVector3 btTorque;
		CastBulletVector3(torque, &btTorque);
		mBulletBody->applyTorque(btTorque);
	}

	void RigidBody::ApplyTorqueImpulse(const glm::vec3& torqueImpulse)
	{
		btVector3 btTorqueImpulse;
		CastBulletVector3(torqueImpulse, &btTorqueImpulse);
		mBulletBody->applyTorque(btTorqueImpulse);
	}

	void RigidBody::CancelForces()
	{
		mBulletBody->clearForces();
		mBulletBody->setLinearVelocity(btVector3(0, 0, 0));
		mBulletBody->setAngularVelocity(btVector3(0, 0, 0));
	}

	void RigidBody::SetOrigin(glm::vec3 position, glm::vec3 orientation)
	{
		btTransform transform;
		
		btVector3 newPosition;
		CastBulletVector3(position, &newPosition);
		
		btQuaternion newDirection;
		CastBulletQuaternion(glm::quat(orientation), &newDirection);

		transform.setOrigin(newPosition);
		transform.setRotation(newDirection);

	
		mBulletBody->setActivationState(ACTIVE_TAG);
		mBulletBody->setWorldTransform(transform);
	}

	
}
