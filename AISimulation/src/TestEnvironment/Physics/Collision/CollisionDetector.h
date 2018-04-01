#ifndef COLLISION_DETECTOR_H
#define COLLISION_DETECTOR_H

#include "../PhysicsObject/PhysicObject.h"
#include "Colliders/Collider.h"
#include "CollisionTest.h"

class CollisionDetector
{
public:

	// IsCollision
	bool IsCollision(PhysicObject& objectA, PhysicObject& objectB, Contacts& outContacts)
	{
		if (!objectA.HasCollider() || !objectB.HasCollider())
		{
			return false;
		}

		// Test collision between colliders
		const Collider& colliderA = objectA.GetCollider();
		const Collider& colliderB = objectB.GetCollider();

		return Test(colliderA, colliderB, outContacts);
	}

private:

	// Test collision
	bool Test(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
	{
		switch (GetCollisionTestType(colliderA, colliderB))
		{
		case CollisionTest::Type::NONE:
			return false;

		case CollisionTest::Type::AABB_AABB:
			return CollisionTest::AABB_AABB(colliderA, colliderB, outContacts);
		case CollisionTest::Type::AABB_PLANE:
			return CollisionTest::AABB_Plane(colliderA, colliderB, outContacts);
		case CollisionTest::Type::AABB_SPHERE:
			return CollisionTest::AABB_Sphere(colliderA, colliderB, outContacts);
		case CollisionTest::Type::SPHERE_PLANE:
			return CollisionTest::Sphere_Plane(colliderA, colliderB, outContacts);
		case CollisionTest::Type::SPHERE_SPHERE:
			return CollisionTest::Sphere_Sphere(colliderA, colliderB, outContacts);
		default:
			assert(false);
			break;
		}

		return false;
	}

	// Get collision test
	CollisionTest::Type GetCollisionTestType(const Collider& colliderA, const Collider& colliderB)
	{
		CollisionTest::Type testType = CollisionTest::Type::NONE;

		ColliderType colliderTypeA = colliderA.GetType();
		ColliderType colliderTypeB = colliderB.GetType();

		switch (colliderTypeA)
		{
			case ColliderType::AABB:
			{
				switch (colliderTypeB)
				{
				case ColliderType::AABB:
				{
					testType = CollisionTest::Type::AABB_AABB;
					break;
				}
				case ColliderType::PLANE:
				{
					testType = CollisionTest::Type::AABB_PLANE;
					break;
				}
				case ColliderType::SPHERE:
				{
					testType = CollisionTest::Type::AABB_SPHERE;
					break;
				}
				default:
					assert(false);
					break;
				}
				break;
			}
			case ColliderType::PLANE:
			{
				switch (colliderTypeB)
				{
				case ColliderType::AABB:
				{
					testType = CollisionTest::Type::AABB_PLANE;
					break;
				}
				case ColliderType::PLANE:
				{
					testType = CollisionTest::Type::NONE;
					break;
				}
				case ColliderType::SPHERE:
				{
					testType = CollisionTest::Type::SPHERE_PLANE;
					break;
				}
				default:
					assert(false);
					break;
				}
				break;
			}
			case ColliderType::SPHERE:
			{
				switch (colliderTypeB)
				{
				case ColliderType::AABB:
				{
					testType = CollisionTest::Type::AABB_SPHERE;
					break;
				}
				case ColliderType::PLANE:
				{
					testType = CollisionTest::Type::SPHERE_PLANE;
					break;
				}
				case ColliderType::SPHERE:
				{
					testType = CollisionTest::Type::SPHERE_SPHERE;
					break;
				}
				default:
					assert(false);
					break;
				}
				break;
			}
			
			default:
			assert(false);
			break;
		}

		return testType;
	}
};
#endif // !COLLISION_DETECTOR_H

