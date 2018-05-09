#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Render/Renderable.h"
#include "MathGeom.h"

#include "Physics/PhysicsObject/IPhysicObject.h"

#include "Transform.h"

class GameObject
{
	// physic object
	IPhysicObject* physicObject{ nullptr };

	// renderable
	std::shared_ptr<Renderable> renderable;

public:

	// Transform
	Transform transform;

public:

	// Set position
	void SetPosition(MathGeom::Vector3& pos) 
	{
		transform.position = pos;

		if (physicObject)
		{
			physicObject->SetTransform(transform);
		}
	}

	// Set renderable
	void SetRenderable(std::shared_ptr<Renderable>& renderable_) { renderable = renderable_; }

	// Set visible
	void SetVisible(bool visible) { renderable->SetVisible(visible); }

	// Set physics object
	void SetPhysicObject(IPhysicObject* physicsObj) { physicObject = physicsObj; }

	// Render
	void Render(const glm::mat4& viewProjection)
	{
		renderable->Render(viewProjection, transform);
	}

protected:

	// Add Force
	void AddForce(const MathGeom::Vector3& force)
	{
		assert(physicObject);
		if (physicObject)
		{
			physicObject->AddForce(force * physicObject->Mass());
		}
	}

	// Set stationary
	void SetStationary(bool stationary)
	{
		physicObject->SetStationary(stationary);
	}

	// Get velocity
	const MathGeom::Vector3& Velocity() const { return physicObject->Velocity(); }
};

#endif // !GAME_OBJECT_H


