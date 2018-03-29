#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include <vector>

#include "../GameObject.h"

#include "Collision/CollisionDetector.h"
#include "Forces/Forces.h"
#include "PhysicsObject/PhysicObjectDesc.h"
#include "PhysicsObject/Particle.h"

class PhysicsEngine
{
	// physic objects
	using PhysicObjects = std::vector<PhysicObject*>;
	PhysicObjects physicObjects;

	// particles
	static const int MAX_PARTICLE_OBJECTS = 100;
	using Particles = std::vector<Particle>;
	Particles particles;

	// forces map
	using ForcesMapEntryFirst = PhysicObject*;
	using ForcesMapEntrySecond = std::vector<std::unique_ptr<IForce>>;
	using ForcesMap = std::map<ForcesMapEntryFirst, ForcesMapEntrySecond>;
	ForcesMap forcesMap;

	// Collision detector
	CollisionDetector collisionDetector;

public:

	// Init
	void Init()
	{
		particles.reserve(MAX_PARTICLE_OBJECTS);
	}

	// Add physic to the game object
	IPhysicObject* AddPhysics(GameObject& object, const PhysicObjectDesc& desc)
	{
		switch (desc.type)
		{
			case PhysicObjectType::PARTICLE:
			{
				assert(particles.size() < MAX_PARTICLE_OBJECTS);
				if (particles.size() < MAX_PARTICLE_OBJECTS)
				{
					particles.emplace_back(Particle(object, desc));
					physicObjects.emplace_back(&particles.back());
				}
				break;
			}
			default:
			{
				printf("PhysicObjectType (%d) not handled", (int)desc.type);
				return nullptr;
			}
		}

		// set collider
		SetCollider(physicObjects.back(), desc.colliderDesc);

		// register gravity force
		if (desc.isAffectedByGravity)
		{
			std::unique_ptr<IForce> gravityForce = std::make_unique<GravityForce>(MathGeom::Vector3(0.0f, -9.8f, 0.0f));
			RegisterForce(physicObjects.back(), gravityForce);
		}
		
		// attach the physics object to the game object
		object.SetPhysicObject(physicObjects.back());

		return physicObjects.back();
	}

	// Update
	void Update(float deltaTime)
	{
		if (physicObjects.size() == 0)
		{
			// nothing to update
			return;
		}

		// add forces
		AddForces();
		
		// integrate
		Integrate(deltaTime);

		// handle collision
		HandleCollisions();
	}

	// Debug render
	void DebugRender(const glm::mat4& viewProjection)
	{
		// debug render colliders
		DebugRenderColliders(viewProjection);
	}

private:

	// Set collider
	void SetCollider(PhysicObject* physicObject, const std::unique_ptr<ColliderDesc>& colliderDesc)
	{
		assert(colliderDesc);
		if (colliderDesc)
		{
			std::unique_ptr<Collider> collider;

			switch (colliderDesc->type)
			{
			case ColliderType::AABB:
			{
				collider = std::make_unique<AABBCollider>(colliderDesc->transform);
				break;
			}
			case ColliderType::PLANE:
			{
				MathGeom::Vector3 pointA = static_cast<PlaneColliderDesc*>(colliderDesc.get())->pointA;
				MathGeom::Vector3 pointB = static_cast<PlaneColliderDesc*>(colliderDesc.get())->pointB;
				MathGeom::Vector3 pointC = static_cast<PlaneColliderDesc*>(colliderDesc.get())->pointC;

				collider = std::make_unique<PlaneCollider>(pointA, pointB, pointC, colliderDesc->transform);

				break;
			}
			case ColliderType::SPHERE:
			{
				collider = std::make_unique<SphereCollider>(colliderDesc->transform);
				break;
			}
			default:
				assert(false);
				break;
			}

			physicObject->SetCollider(collider);
		}
	}

	// Register force
	void RegisterForce(PhysicObject* physicObject, std::unique_ptr<IForce>& force)
	{
		auto forcesMapEntry = forcesMap.find(physicObject);
		if (forcesMapEntry == forcesMap.end())
		{
			forcesMap.insert(std::pair<ForcesMapEntryFirst, ForcesMapEntrySecond>(physicObject, ForcesMapEntrySecond()));
			forcesMapEntry = forcesMap.find(physicObject);
		}

		forcesMapEntry->second.emplace_back(std::move(force));
	}

	// Add forces
	void AddForces()
	{
		for (auto& forceEntry : forcesMap)
		{
			PhysicObject& physicObject = *forceEntry.first;
			for (auto& force : forceEntry.second)
			{
				force->AddTo(physicObject);
			}
		}
	}

	// Integrate each physic object
	void Integrate(float deltaTime)
	{
		for (auto physicObject : physicObjects)
		{
			physicObject->Integrate(deltaTime);
		}
	}

	// Handle collisions
	void HandleCollisions()
	{
		// TO-DO: replace brute force approach by a broad phase collision detection using spatial partitioning techniques
		for (size_t i = 0; i < physicObjects.size() - 1; i++)
		{
			for (size_t j = i + 1; j < physicObjects.size(); j++)
			{
				if (collisionDetector.IsCollision(*physicObjects[i], *physicObjects[j]))
				{
					printf("Collision!\n");
				}
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
// DEBUG
public:

	static bool s_renderColliders;

private:

	// Render colliders
	void DebugRenderColliders(const MathGeom::Matrix4& viewProjection)
	{
		if (s_renderColliders)
		{
			for (size_t i = 0; i < physicObjects.size(); i++)
			{
				physicObjects[i]->DebugRenderCollider(viewProjection);
			}
		}
	}

// DEBUG
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

};

bool PhysicsEngine::s_renderColliders = false;

#endif // !PHYSICS_ENGINE

