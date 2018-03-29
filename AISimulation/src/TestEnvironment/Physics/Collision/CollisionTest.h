#ifndef COLLISION_TEST_H
#define COLLISION_TEST_H

#include "Colliders/Collider.h"

#include "Colliders/AABBCollider.h"
#include "Colliders/PlaneCollider.h"
#include "Colliders/SphereCollider.h"

#include "ClosestPointOn.h"
#include "DistanceTo.h"

class CollisionTest
{
public:

	enum class Type
	{
		NONE,
		AABB_AABB,
		AABB_PLANE,
		AABB_SPHERE,
		SPHERE_PLANE,
		SPHERE_SPHERE,
	};

	static bool AABB_AABB(const Collider& colliderA, const Collider& colliderB)
	{
		const AABBCollider& boxA = *static_cast<const AABBCollider*>(&colliderA);
		const AABBCollider& boxB = *static_cast<const AABBCollider*>(&colliderB);

		// check overlap in all the axes
		if (std::fabsf(boxA.transform.position.x - boxB.transform.position.x) > std::fabsf(boxA.halfSize.x + boxB.halfSize.x)) 
			return false;
		if (std::fabsf(boxA.transform.position.y - boxB.transform.position.y) > std::fabsf(boxA.halfSize.y + boxB.halfSize.y)) 
			return false;
		if (std::fabsf(boxA.transform.position.z - boxB.transform.position.z) > std::fabsf(boxA.halfSize.z + boxB.halfSize.z)) 
			return false;

		// overlap in all the axes
		return true;
	}

	static bool AABB_Plane(const Collider& colliderA, const Collider& colliderB)
	{
		const AABBCollider& box = colliderA.GetType() == ColliderType::AABB ? *static_cast<const AABBCollider*>(&colliderA)
			: *static_cast<const AABBCollider*>(&colliderB);

		const PlaneCollider& plane = colliderA.GetType() == ColliderType::PLANE ? *static_cast<const PlaneCollider*>(&colliderA)
			: *static_cast<const PlaneCollider*>(&colliderB);

		auto& bCenter = box.transform.position; // box center
		auto& bExtent = box.halfSize; // box extents
		auto& pNormal = plane.normal;

		// projection interval of AABB onto separating axis L(t) = boxCenter  + t*plane,normal
		float r = bExtent.x * std::fabsf(pNormal.x) + bExtent.y * std::fabsf(pNormal.y) + bExtent.z * std::fabsf(pNormal.z);

		// distance from bCenter to plane
		float distance = DistanceTo::Plane(bCenter, plane);

		// intersection when the distance is in [-r, r]
		return std::fabs(distance) <= r;
	}

	static bool AABB_Sphere(const Collider& colliderA, const Collider& colliderB)
	{
		const AABBCollider& box = colliderA.GetType() == ColliderType::AABB ? *static_cast<const AABBCollider*>(&colliderA) 
																			: *static_cast<const AABBCollider*>(&colliderB);

		const SphereCollider& sphere = colliderA.GetType() == ColliderType::SPHERE ? *static_cast<const SphereCollider*>(&colliderA) 
																				   : *static_cast<const SphereCollider*>(&colliderB);

		// closest point on AABB from sphere center
		auto& sphereCenter = sphere.transform.position;
		auto& closestPointOnAABB = ClosestPointOn::AABB(sphereCenter, box);

		// intersection if the squared distance between them is less than sphere squared radius
		auto fromSphereToAABB = closestPointOnAABB - sphereCenter;
		float distanceSq = fromSphereToAABB.x * fromSphereToAABB.x + fromSphereToAABB.y * fromSphereToAABB.y + fromSphereToAABB.z * fromSphereToAABB.z;
		return distanceSq <= sphere.radius * sphere.radius;
	}

	static bool Sphere_Plane(const Collider& colliderA, const Collider& colliderB)
	{
		const SphereCollider& sphere= colliderA.GetType() == ColliderType::SPHERE ? *static_cast<const SphereCollider*>(&colliderA)
																				  : *static_cast<const SphereCollider*>(&colliderB);

		const PlaneCollider& plane = colliderA.GetType() == ColliderType::PLANE ? *static_cast<const PlaneCollider*>(&colliderA)
																				: *static_cast<const PlaneCollider*>(&colliderB);

		// All points in a Plane satisfy dot(plane.normal, p) = plane.d
		// Therefore, if p = sphere.center there will be an intersection if the distance from the plane is less than the sphere radius
		float distance = DistanceTo::Plane(sphere.transform.position, plane);
		return std::fabsf(distance) < sphere.radius;
	}

	static bool Sphere_Sphere(const Collider& colliderA, const Collider& colliderB)
	{
		const SphereCollider& sphereA = *static_cast<const SphereCollider*>(&colliderA); 
		const SphereCollider& sphereB = *static_cast<const SphereCollider*>(&colliderB);
		
		// squared distance between centers by using the dot product
		auto fromAtoB = sphereA.transform.position - sphereB.transform.position;
		float distanceSq = glm::dot(fromAtoB, fromAtoB);

		float radiusSum = sphereA.radius + sphereB.radius;
		return distanceSq <= radiusSum * radiusSum;
	}
};

#endif // !COLLISION_TEST_H

