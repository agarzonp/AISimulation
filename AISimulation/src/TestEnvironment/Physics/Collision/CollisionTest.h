#ifndef COLLISION_TEST_H
#define COLLISION_TEST_H

#include <array>

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

	static bool AABB_AABB(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
	{
		const AABBCollider& boxA = *static_cast<const AABBCollider*>(&colliderA);
		const AABBCollider& boxB = *static_cast<const AABBCollider*>(&colliderB);

		const MathGeom::Vector3& halfSizeA = boxA.halfSize;
		const MathGeom::Vector3& halfSizeB = boxB.halfSize;
		MathGeom::Vector3 fromAToB = boxB.transform.position - boxA.transform.position;

		// check overlap in all the axes (Separating Axis Tests - SAT)
		MathGeom::Vector3 axes[3] = { {1.0f, 0.0f, 0.0f}, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };
		float bestOverlap = std::numeric_limits<float>::max();
		MathGeom::Vector3& bestOverlapAxis = axes[0];
		for (auto& axis : axes)
		{
			float overlap = GetOverlap_SAT(axis, fromAToB, halfSizeA, halfSizeB);
			if (overlap < 0)
			{
				// no overlap
				return false;
			}
			
			if (overlap < bestOverlap)
			{
				bestOverlap = overlap;
				bestOverlapAxis = axis;
			}
		}
		
		// overlap in all the axes, so boxes are in contact
		// determine the contact (vertex-face contact)
		
		MathGeom::Vector3 contactNormal = bestOverlapAxis;
		if (MathGeom::Dot(contactNormal, fromAToB) > 0)
		{
			// Make sure that normal is always from A´s perspective, so we always look for the vertex in B interpenetrating with A
			contactNormal *= -1.0f;
		}

		// contact vertex is the closest vertex to center
		auto vertices = boxB.Vertices();
		float bestDistSq = std::numeric_limits<float>::max();
		MathGeom::Vector3& contactVertex = vertices[0];
		for (auto& vertex : vertices)
		{
			MathGeom::Vector3 toA = boxA.transform.position - vertex;
			float distSq = MathGeom::Dot(toA, toA);
			if (distSq < bestDistSq)
			{
				bestDistSq = distSq;
				contactVertex = vertex;
			}
		}
		
		ContactData contact;
		contact.normal = contactNormal;
		contact.penetration = bestOverlap;
		contact.point = contactVertex;

		outContacts.push_back(contact);
		return true;
	}

	static float GetOverlap_SAT(const MathGeom::Vector3& axis, const MathGeom::Vector3& fromAtoB, const MathGeom::Vector3& halfSizeA, const MathGeom::Vector3& halfSizeB)
	{
		float distance = std::fabsf(MathGeom::Dot(axis, fromAtoB));
		return MathGeom::Dot(axis, halfSizeA) + MathGeom::Dot(axis, halfSizeB) - distance;
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

	static bool AABB_Plane(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
	{
		if (!AABB_Plane(colliderA, colliderB))
		{
			// no collision at all
			return false;
		}

		const AABBCollider& box = colliderA.GetType() == ColliderType::AABB ? *static_cast<const AABBCollider*>(&colliderA)
			: *static_cast<const AABBCollider*>(&colliderB);

		const PlaneCollider& plane = colliderA.GetType() == ColliderType::PLANE ? *static_cast<const PlaneCollider*>(&colliderA)
			: *static_cast<const PlaneCollider*>(&colliderB);

		// Generate a contact for each vertex of the box penetrating the plane 
		std::array<MathGeom::Vector3, 8> vertices = box.Vertices();
		for (auto& vertex : vertices)
		{
			float distance = DistanceTo::Plane(vertex, plane);
			if (distance < 0.0f)
			{
				// vertext penetrated the plane, so create a contact (vertex-face contact)
				ContactData contact; 
				contact.normal = plane.normal;
				contact.penetration = -distance;
				contact.point = vertex; 
				outContacts.push_back(contact);
			}
		}

		return true;
	}

	static bool AABB_Sphere(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
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
		bool isCollision = distanceSq <= sphere.radius * sphere.radius;
		if (isCollision)
		{
			// generate contact (vertext-face, edge-face or face-face contact)
			ContactData contact;
			contact.normal = colliderA.GetType() == ColliderType::SPHERE ? glm::normalize(-fromSphereToAABB) : glm::normalize(fromSphereToAABB);
			contact.penetration = std::sqrtf(distanceSq) - sphere.radius;
			contact.point = closestPointOnAABB;

			outContacts.push_back(contact);
		}

		return isCollision;
	}

	static bool Sphere_Plane(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
	{
		const SphereCollider& sphere= colliderA.GetType() == ColliderType::SPHERE ? *static_cast<const SphereCollider*>(&colliderA)
																				  : *static_cast<const SphereCollider*>(&colliderB);

		const PlaneCollider& plane = colliderA.GetType() == ColliderType::PLANE ? *static_cast<const PlaneCollider*>(&colliderA)
																				: *static_cast<const PlaneCollider*>(&colliderB);

		// All points in a Plane satisfy dot(plane.normal, p) = plane.d
		// Therefore, if p = sphere.center there will be an intersection if the distance from the plane is less than the sphere radius
		float distance = DistanceTo::Plane(sphere.transform.position, plane);
		bool isCollision = std::fabsf(distance) < sphere.radius;
		if (isCollision)
		{
			// generate contact (face-face contact)
			ContactData contact;
			contact.normal = plane.normal;
			contact.penetration = -(distance - sphere.radius);
			contact.point = sphere.transform.position - plane.normal * contact.penetration;
			
			outContacts.push_back(contact);
		}

		return isCollision;
	}

	static bool Sphere_Sphere(const Collider& colliderA, const Collider& colliderB, Contacts& outContacts)
	{
		const SphereCollider& sphereA = *static_cast<const SphereCollider*>(&colliderA); 
		const SphereCollider& sphereB = *static_cast<const SphereCollider*>(&colliderB);
		
		// squared distance between centers by using the dot product
		auto fromBtoA = sphereA.transform.position - sphereB.transform.position;
		float distanceSq = glm::dot(fromBtoA, fromBtoA);

		float radiusSum = sphereA.radius + sphereB.radius;

		bool isCollision = distanceSq <= radiusSum * radiusSum;
		if (isCollision)
		{
			// generate contact (face-face contact)
			ContactData contact;
			contact.normal = glm::normalize(fromBtoA);
			contact.penetration = sphereA.radius + sphereB.radius - glm::length(fromBtoA);
			contact.point = sphereB.transform.position + contact.normal*sphereB.radius;
		}

		return isCollision;
	}
};

#endif // !COLLISION_TEST_H

