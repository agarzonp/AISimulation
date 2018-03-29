#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include "Collider.h"

class AABBCollider : public Collider
{
public:

	// half size
	MathGeom::Vector3 halfSize;

	// Constructor
	AABBCollider() = delete;
	AABBCollider(Transform& transform)
		: Collider(ColliderType::AABB, transform)
	{
		halfSize = transform.scale;
	}

	// Debug Render
	void DebugRender(const glm::mat4& viewProjection) final
	{
		Transform AABBTransform = transform;
		AABBTransform.scale = halfSize * 1.001f;

		RenderUtils::RenderCube(viewProjection, AABBTransform, 0xFFFFFF);
	}
};

#endif // !AABB_COLLIDER_H
