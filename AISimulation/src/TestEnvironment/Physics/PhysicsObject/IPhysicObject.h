#ifndef IPHYSIC_OBJECT_H
#define IPHYSIC_OBJECT_H

#include <vector>

#include "../Collision/Colliders/Collider.h"

class IPhysicObject
{
public:

	// Add force
	virtual void AddForce(const MathGeom::Vector3& force) = 0;

	// Set stationary
	virtual void SetStationary(bool stationary) = 0;

	// Mass
	virtual float Mass() = 0;

	// Velocity
	virtual const MathGeom::Vector3& Velocity() const = 0;
	virtual MathGeom::Vector3& Velocity() = 0;

	// Set Transform
	virtual void SetTransform(const Transform& transform) = 0;

	// Set collider
	virtual void SetCollider(std::unique_ptr<Collider>& collider) = 0;
};

#endif // !IPHYSIC_OBJECT_H

