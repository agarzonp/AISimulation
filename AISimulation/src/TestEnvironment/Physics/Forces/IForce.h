#ifndef IFORCE_H
#define IFORCE_H

#include "../PhysicsObject/PhysicObject.h"

class IForce
{
public:
	virtual void AddTo(PhysicObject& physicObject) = 0;
};

#endif // !IFORCE_H

