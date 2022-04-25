#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//#include <BulletSoftBody/btSoftBody.h>

#include <iCollisionBody.h>
#include <gdpPhysics/interfaces/iRigidBody.h>
//#include <GdpPhysics/interfaces/iSoftBody.h>
#include <gdpPhysics/interfaces/iShape.h>
//#include <gdpPhysics/interfaces/iVehicle.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
};

namespace gdpPhysics
{
	// GLM -> Bullet
	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out);
	void CastBulletVector3(const glm::vec3& in, btVector3* out);
	void CastBulletScalar(const float in, btScalar* out);

	// Bullet -> GLM
	void CastGLMQuaternion(const btQuaternion& in, glm::quat* out);
	void CastGLMVector3(const btVector3& in, glm::vec3* out);
	void CastGLMMatrix4(const btTransform& in, glm::mat4* out);
	void CastGLMPosition(const btTransform& in, glm::vec3* out);
	void CastFloat(const btScalar& in, float* out);
	void CastTransform(const btTransform& in, Transform* out);

	// Interface -> Bullet
	btRigidBody* CastBulletRigidBody(iCollisionBody* body);
	//btSoftBody* CastBulletSoftBody(iCollisionBody* body);
	//btRaycastVehicle* CastBulletVehicle(iVehicle* vehicle);
	//btVehicleRaycaster* CastBulletVehicleRaycaster(iVehicleRaycaster* raycaster);
	btCollisionShape* CastBulletShape(iShape* shape);
}