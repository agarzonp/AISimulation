#ifndef COLLIDER_H
#define COLLIDER_H

enum class ColliderType
{
	INVALID,

	AABB,
	PLANE,
	SPHERE
	
};

class Collider
{
	// collider type
	ColliderType type{ ColliderType::INVALID };

public:

	// Transform
	Transform transform;

public:
	
	// Constructors
	Collider() = default;
	Collider(ColliderType colliderType, Transform& transform_) 
		: type(colliderType)
		, transform(transform_)
	{
	}

	// Destructor
	virtual ~Collider() {}

	// Get Type
	ColliderType GetType() const { return type; }

	// Set transform
	void SetTransform(const Transform& transform_) { transform = transform_; }

	// Debug Render
	virtual void DebugRender(const glm::mat4& viewProjection) = 0;
};

#endif // !COLLIDER_H

