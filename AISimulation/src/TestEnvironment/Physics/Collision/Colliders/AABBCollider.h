#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include <array>

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
	
	// Get vertices
	std::array<MathGeom::Vector3, 8> Vertices() const
	{
		auto& center = transform.position;

		std::array<MathGeom::Vector3, 8> vertices;
		vertices[0] = { center.x + halfSize.x, center.y + halfSize.y, center.z + halfSize.z };
		vertices[1] = { center.x + halfSize.x, center.y - halfSize.y, center.z + halfSize.z };
		vertices[2] = { center.x - halfSize.x, center.y - halfSize.y, center.z + halfSize.z };
		vertices[3] = { center.x - halfSize.x, center.y + halfSize.y, center.z + halfSize.z };
		vertices[4] = { center.x + halfSize.x, center.y + halfSize.y, center.z - halfSize.z };
		vertices[5] = { center.x + halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
		vertices[6] = { center.x - halfSize.x, center.y - halfSize.y, center.z - halfSize.z };
		vertices[7] = { center.x - halfSize.x, center.y + halfSize.y, center.z - halfSize.z };
		return vertices;
	}

	// Debug Render
	void DebugRender(const glm::mat4& viewProjection) final
	{
		Transform AABBTransform;
		AABBTransform.position = transform.position;
		AABBTransform.scale = halfSize * 1.001f;

		RenderUtils::RenderCube(viewProjection, AABBTransform, 0xFFFFFF);
	}
};

#endif // !AABB_COLLIDER_H
