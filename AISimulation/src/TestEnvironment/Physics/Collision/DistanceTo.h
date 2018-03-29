#ifndef DISTANCE_TO_H
#define DISTANCE_TO_H

class DistanceTo
{
public:

	// Distance from point to plane
	static float Plane(const MathGeom::Vector3& point, const PlaneCollider& plane)
	{
		return glm::dot(point, plane.normal) - plane.d;
	}
};

#endif // !DISTANCE_FROM_POINT_TO_H

