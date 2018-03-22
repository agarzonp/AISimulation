#ifndef COLLISION_TEST_H
#define COLLISION_TEST_H

#include "Colliders/Collider.h"
#include "Colliders/SphereCollider.h"

class CollisionTest
{
public:

	enum class Type
	{
		NONE,
		SPHERE_SPHERE
	};

	static bool SphereShere(const SphereCollider& sphereA, const SphereCollider& sphereB)
	{
		// squared distance between centers by using the dot product
		auto fromAtoB = sphereA.transform.position - sphereB.transform.position;
		float distanceSq = glm::dot(fromAtoB, fromAtoB);

		float radiusSum = sphereA.radius + sphereB.radius;
		return distanceSq <= radiusSum * radiusSum;
	}
};

#endif // !COLLISION_TEST_H

