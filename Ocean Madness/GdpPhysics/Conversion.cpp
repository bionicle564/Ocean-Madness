#include "Conversion.h"
#include "GdpPhysics/RigidBody.h"
#include <gdpPhysics/interfaces/shapes.h>

namespace gdpPhysics 
{
	void CastBulletQuaternion(const glm::quat& in, btQuaternion* out)
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
		out->setW(in.w);
	}

	void CastBulletVector3(const glm::vec3& in, btVector3* out)
	{
		out->setX(in.x);
		out->setY(in.y);
		out->setZ(in.z);
	}

	void CastBulletScalar(const float in, btScalar* out)
	{
		*out = in;
	}

	void CastGLMQuaternion(const btQuaternion& in, glm::quat* out)
	{
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
		CastFloat(in.w(), &out->w);
	}

	void CastGLMVector3(const btVector3& in, glm::vec3* out)
	{
		CastFloat(in.x(), &out->x);
		CastFloat(in.y(), &out->y);
		CastFloat(in.z(), &out->z);
	}

	void CastGLMMatrix4(const btTransform& in, glm::mat4* out)
	{
		//empty
	}

	void CastGLMPosition(const btTransform& in, glm::vec3* out)
	{
		CastFloat(in.getOrigin().x(), &out->x);
		CastFloat(in.getOrigin().y(), &out->y);
		CastFloat(in.getOrigin().z(), &out->z);
	}

	void CastFloat(const btScalar& in, float* out)
	{
		*out = in;
	}

	void CastTransform(const btTransform& in, Transform* out)
	{
		CastGLMQuaternion(in.getRotation(), &out->rotation);
		btVector3 position = in.getBasis() * in.getOrigin();
		CastGLMVector3(position, &out->position);
	}

	btRigidBody* CastBulletRigidBody(iCollisionBody* body)
	{
		return dynamic_cast<RigidBody*>(body)->GetBulletBody();
	}

	btCollisionShape* CastBulletShape(iShape* shape)
	{
		switch (shape->GetShapeType())
		{
		case ShapeTypeBox:
		{
			BoxShape* box = BoxShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(box->GetHalfExtents(), &halfExtents);

			btBoxShape* btBox = new btBoxShape(halfExtents);

			return btBox;
		}
		break;

		case ShapeTypeCylinder:
		{
			CylinderShape* cylinder = CylinderShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(cylinder->GetHalfExtents(), &halfExtents);

			btCylinderShape* btCylinder = new btCylinderShape(halfExtents);

			return btCylinder;
		}
		case ShapeTypeCapsule:
		{
			CapsuleShape* capsule = CapsuleShape::Cast(shape);

			btVector3 halfExtents;

			CastBulletVector3(capsule->GetHalfExtents(), &halfExtents);

			btCapsuleShape* btCapsule = new btCapsuleShape(halfExtents.getX(), halfExtents.getY());

			return btCapsule;
		}
		break;
		
		case ShapeTypeCone:
		{
			ConeShape* cone = ConeShape::Cast(shape);
			
			glm::vec3 info = cone->GetInformation();
			
			btConeShape* btCone = new btConeShape(info.x, info.y);

			return btCone;
		}
		break;

		case ShapeTypePlane:
		{
			PlaneShape* plane = PlaneShape::Cast(shape);

			btVector3 normal;
			btScalar dot;

			CastBulletVector3(plane->GetNormal(), &normal);
			CastBulletScalar(plane->GetDotProduct(), &dot);

			btStaticPlaneShape* btPlane = new btStaticPlaneShape(normal, dot);

			return btPlane;
		}
		break;

		case ShapeTypeSphere:
		{
			SphereShape* sphere = SphereShape::Cast(shape);

			btScalar radius;

			CastBulletScalar(sphere->GetRadius(), &radius);

			btSphereShape* btSphere = new btSphereShape(radius);

			return btSphere;
		}
		break;

		default:
			break;
		}
	}


}

