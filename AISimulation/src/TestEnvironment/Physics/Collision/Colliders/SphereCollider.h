#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"

class SphereCollider : public Collider
{
public:

	// radius
	float radius{ 1.0f };

	// Constructor
	SphereCollider() = delete;
	SphereCollider(float radius_, Transform& transform)
		: Collider(ColliderType::SPHERE, transform)
		, radius(radius_)
	{
	}
};

#endif // !SPHERE_COLLIDER_H

