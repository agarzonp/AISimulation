#ifndef PARTICLE_H
#define PARTICLE_H

#include "PhysicObject.h"

class Particle : public PhysicObject
{
public:

	Particle() = default;
	Particle(GameObject& object, const PhysicObjectDesc& desc)
		: PhysicObject(object, desc)
	{
	}

};

#endif // !PARTICLE

