#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include "PhysicsObject/PhysicObject.h"

#include "Collision/ContactData.h"
#include "Collision/ContactsGenerator.h"
#include "Collision/ContactsResolver.h"

class CollisionManager
{
	// maximum of contacts
	size_t maxContacts{ 100 };

	// contacts
	Contacts contacts;

	// Contacts generator
	ContactGenerator contactsGenerator;

	// Contatcs resolver
	ContactsResolver contactsResolver;

public:

	// Init
	void Init(size_t maxContacts)
	{
		this->maxContacts = maxContacts;
		contacts.reserve(maxContacts);
	}

	// Update
	void Update(PhysicObjects& objects, float deltaTime)
	{
		contacts.clear();

		// Collision Detection
		// TO-DO: replace brute force approach by a broad phase collision detection using spatial partitioning techniques
		for (size_t i = 0; i < objects.size() - 1; i++)
		{
			for (size_t j = i + 1; j < objects.size(); j++)
			{
				// generate contacts between colliding pair of objects
				if (contactsGenerator.GenerateContacts(*objects[i], *objects[j]))
				{
					printf("Collision!\n");

					// append generated contacts
					contacts.insert(contacts.end(), contactsGenerator.GetContacts().begin(), contactsGenerator.GetContacts().end());
				}
			}
		}

		if (contacts.size() > 0)
		{
			assert(contacts.size() <= maxContacts);
			printf("Number of contacts: %d\n", contacts.size());

			// resolve contacts
			contactsResolver.Resolve(contacts, deltaTime);
		}
	}
};

#endif // !COLLISION_MANAGER_H

