#ifndef SPHERE_RENDERABLE_H
#define SPHERE_RENDERABLE_H

#include "Renderable.h"

class SphereRenderable : public Renderable
{
	// radius
	float radius{ 1.0f };

public:
	// Constructors
	SphereRenderable() = default;
	SphereRenderable(float radius_, Mesh* mesh_, Shader& shader_, const glm::vec4& color_)
		: Renderable(mesh_, shader_, color_)
		, radius(radius_)
	{
	}

protected:

	glm::mat4 ModelMatrix(const Transform& transform) override
	{			
		return glm::scale(Renderable::ModelMatrix(transform), radius * transform.scale);
	}
};

#endif // !SPHERE_RENDERABLE_H

