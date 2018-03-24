#ifndef GRAVITY_FORCE_H
#define GRAVITY_FORCE_H

#include "IForce.h"

class GravityForce : public IForce
{
	MathGeom::Vector3 gravity;

public:

	// Constructor
	GravityForce(const MathGeom::Vector3& gravity_) : gravity(gravity_){}

	// Add to
	void AddTo(PhysicObject& physicObject) final
	{
		float mass = physicObject.Mass();

		if (mass > 0.0f)
		{
			physicObject.AddForce(gravity * mass);
		}
	}
};
#endif // !GRAVITY_FORCE_H

