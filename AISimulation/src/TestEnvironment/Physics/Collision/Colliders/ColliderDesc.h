#ifndef COLLIDER_DESC_H
#define	COLLIDER_DESC_H

// Collider desc
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

// AABB collider desc
struct AABBColliderDesc : public ColliderDesc
{
	AABBColliderDesc(Transform& transform)
		: ColliderDesc(ColliderType::AABB, transform)
	{
	}
};

// Plane collider desc
struct PlaneColliderDesc : public ColliderDesc
{
	MathGeom::Vector3 pointA;
	MathGeom::Vector3 pointB;
	MathGeom::Vector3 pointC;

	PlaneColliderDesc(MathGeom::Vector3& a, MathGeom::Vector3& b, MathGeom::Vector3& c, Transform& transform)
		: ColliderDesc(ColliderType::PLANE, transform)
		, pointA(a)
		, pointB(b)
		, pointC(c)
	{
	}
};

// Sphere collider desc
struct SphereColliderDesc : public ColliderDesc
{
	SphereColliderDesc(Transform& transform)
		: ColliderDesc(ColliderType::SPHERE, transform)
	{
	}
};

#endif // !COLLIDER_DESC_H

