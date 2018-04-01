#ifndef CONTACT_GENERATOR_H
#define CONTACT_GENERATOR_H

#include "ContactData.h"
#include "CollisionDetector.h"

class ContactGenerator
{
	// contacts
	Contacts contacts;

	// Collision detector
	CollisionDetector collisionDetector;

public:
	// constructor
	ContactGenerator() { contacts.reserve(8); }

	// Generate contacts
	bool GenerateContacts(PhysicObject& objectA, PhysicObject& objectB)
	{
		contacts.clear();

		// check collisions and create the contacts
		if (!collisionDetector.IsCollision(objectA, objectB, contacts))
		{	
			// no contacts at all
			return false;
		}

		assert(contacts.size() > 0);

		// make sure that objectA is not the immmovable object
		for (auto& contact : contacts)
		{
			contact.objectA = objectA.InverseMass() > 0 ? &objectA : &objectB;
			contact.objectB = contact.objectA == &objectA ? &objectB : &objectA;

			assert(contact.objectA->InverseMass() > 0);

			if (contact.objectB->InverseMass() <= 0)
			{
				// contact resolution not needed for immovable object
				contact.objectB = nullptr;
			}
		}

		return true;
	}

	// Get contacts
	const Contacts& GetContacts() const { return contacts; }

};

#endif // !CONTACT_GENERATOR_H

