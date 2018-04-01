#ifndef CONTACT_DATA_H
#define CONTACT_DATA_H

#include <vector>

#include "../PhysicsObject/PhysicObject.h"

struct ContactData
{
	// pair of objects in contact
	PhysicObject* objectA{ nullptr };
	PhysicObject* objectB{ nullptr }; // it could be null for immovable objects

	// contact normal (from objectA perspective)
	MathGeom::Vector3 normal;

	// Contact point
	MathGeom::Vector3 point;

	// coefficient of restitution (the speed at which the objects will separate after colliding)
	float restitution{ 1.0f };


	// depth of penetration in the direction of the contact normal
	float penetration{ 0.0f };
};

using Contacts = std::vector<ContactData>;

#endif
