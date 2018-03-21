#ifndef PHYSIC_OBJECT_DESC_H
#define PHYSIC_OBJECT_DESC_H

#include "../Collision/Colliders/ColliderDesc.h"

enum class PhysicObjectType
{
	PARTICLE
};

struct PhysicObjectDesc
{
	PhysicObjectType type;

	float mass{ 0.0f };
	
	MathGeom::Vector3 position;
	MathGeom::Vector3 velocity;
	MathGeom::Vector3 acceleration;

	std::unique_ptr<ColliderDesc> colliderDesc;

	bool isAffectedByGravity{ true };
};

#endif // !PHYSIC_OBJECT_DESC_H

