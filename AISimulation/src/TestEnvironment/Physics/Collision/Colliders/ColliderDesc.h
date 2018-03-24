#ifndef COLLIDER_DESC_H
#define	COLLIDER_DESC_H

// ColliderDesc
struct ColliderDesc
{
	ColliderType type{ ColliderType::INVALID };
	Transform transform;

	ColliderDesc(ColliderType colliderType, Transform transform_) 
		: type(colliderType)
		, transform(transform_)
	{
	}
};

// SphereColliderDesc
struct SphereColliderDesc : public ColliderDesc
{
	float radius{ 1.0f };

	SphereColliderDesc(float radius_, Transform& transform)
		: ColliderDesc(ColliderType::SPHERE, transform)
		, radius(radius_)
	{
	}
};

#endif // !COLLIDER_DESC_H

