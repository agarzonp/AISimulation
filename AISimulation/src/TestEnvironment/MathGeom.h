#ifndef MATH_GEOM_H
#define MATH_GEOM_H

#include "glm/glm.hpp"

namespace MathGeom
{
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Matrix2 = glm::mat2;
	using Matrix3 = glm::mat3;
	using Matrix4 = glm::mat4;

	using Quaternion = glm::quat;

	const double PI = 3.14159265358979323846;

	float Dot(const Vector3& a, const Vector3& b)
	{
		return glm::dot(a, b);
	}

	float DistanceSq(const Vector3& a, const Vector3& b)
	{
		auto fromAtoB = b - a;
		return Dot(fromAtoB, fromAtoB);
	}

}


#endif // !MATH_H

