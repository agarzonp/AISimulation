#ifndef CLOSEST_POINT_ON_H
#define	CLOSEST_POINT_ON_H

class ClosestPointOn
{
public:

	// Closest point on AABB from point p
	static MathGeom::Vector3 AABB(const MathGeom::Vector3& p, const AABBCollider& box)
	{
		float x, y, z;

		// clamp the point to AABB if it is outside the box
		x = std::fmaxf(p.x, box.transform.position.x - box.halfSize.x);
		x = std::fminf(x, box.transform.position.x + box.halfSize.x);

		y = std::fmaxf(p.y, box.transform.position.y - box.halfSize.y);
		y = std::fminf(y, box.transform.position.y + box.halfSize.y);

		z = std::fmaxf(p.z, box.transform.position.z - box.halfSize.z);
		z = std::fminf(z, box.transform.position.z + box.halfSize.z);

		return {x, y, z};
	}

};

#endif // !CLOSEST_POINT_ON_H

