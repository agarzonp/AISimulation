#ifndef RENDER_UTILS_H
#define RENDER_UTILS_H

#include "../MathGeom.h"

#include "Meshes/CubeMesh.h"
#include "Meshes/SphereMesh.h"
#include "Renderable.h"
#include "SphereRenderable.h"

class RenderUtils
{
	// renderables
	static std::unique_ptr<Renderable> cubeRenderable;
	static std::unique_ptr<SphereRenderable> sphereRenderable;

public:
	
	// Init
	static void InitCubeRenderable(Shader& shader, CubeMesh* cubeMesh)
	{
		cubeRenderable = std::make_unique<Renderable>(cubeMesh, shader, glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	}

	static void InitSphereRenderable(Shader& shader, SphereMesh* sphereMesh)
	{
		sphereRenderable = std::make_unique<SphereRenderable>(1.0f, sphereMesh, shader, glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
	}

	// Render cube
	static void RenderCube(const MathGeom::Matrix4& viewProjection, const Transform& transform, unsigned hexColor)
	{
		if (cubeRenderable)
		{
			cubeRenderable->Color() = GetColor(hexColor);
			cubeRenderable->SetVisible(true);
			cubeRenderable->Render(viewProjection, transform);
			cubeRenderable->SetVisible(false);
		}
	}

	// Render cube
	static void RenderSphere(const MathGeom::Matrix4& viewProjection, float radius, const Transform& transform, unsigned hexColor)
	{
		if (sphereRenderable)
		{
			sphereRenderable->Radius() = radius;
			sphereRenderable->Color() = GetColor(hexColor);
			sphereRenderable->SetVisible(true);
			sphereRenderable->Render(viewProjection, transform);
			sphereRenderable->SetVisible(false);
		}
	}

	// Get color from hex value
	static MathGeom::Vector4 GetColor(int hexColor)
	{
		MathGeom::Vector4 color;

		int r = hexColor >> 16;
		int g = (hexColor >> 8) & 0xFF;
		int b = hexColor & 0xFF;

		color.r = float(r / 255);
		color.g = float(g / 255);
		color.b = float(b / 255);
		color.a = 1.0f;

		return color;
	}
};

std::unique_ptr<Renderable> RenderUtils::cubeRenderable;
std::unique_ptr<SphereRenderable> RenderUtils::sphereRenderable;

#endif // !RENDER_UTILS_H

