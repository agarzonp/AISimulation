#ifndef PLANE_COLLIDER_H
#define PLANE_COLLIDER_H

#include "Collider.h"

class PlaneCollider : public Collider
{
public:

	// normal
	MathGeom::Vector3 normal;

	// distance from the origin
	float d;

	// Constructor
	PlaneCollider() = delete;
	PlaneCollider(MathGeom::Vector3& pointA, MathGeom::Vector3& pointB, MathGeom::Vector3& pointC, Transform& transform)
		: Collider(ColliderType::PLANE, transform)
	{
		ComputePlane(pointA, pointB, pointC);
	}

	// Debug Render
	void DebugRender(const glm::mat4& viewProjection) final
	{
		RenderUtils::RenderCube(viewProjection, transform, 0xFFFFFF);
	}

private:

	void ComputePlane(MathGeom::Vector3& pointA, MathGeom::Vector3& pointB, MathGeom::Vector3& pointC)
	{
		normal = glm::normalize(glm::cross(pointB - pointA, pointC - pointA));
		d = glm::dot(normal, pointA);
	}
};

#endif // !SPHERE_COLLIDER_H
