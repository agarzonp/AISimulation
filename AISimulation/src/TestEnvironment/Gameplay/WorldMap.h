#ifndef WORLD_MAP_H
#define WORLD_MAP_H

#include <list>

class WorldMap
{
	// game objects
	std::list<std::shared_ptr<GameObject>> gameObjects;

	// obstacles
	std::list<std::shared_ptr<GameObject>> obstacles;

	// map size
	MathGeom::Vector3 size;
	MathGeom::Vector3 position;

public:

	// getters
	const MathGeom::Vector3& Size() { return size; }
	const MathGeom::Vector3& Position() { return position; }

public:
	
	// Init
	void Init(Shader& shader, CubeMesh& cubeMesh, SphereMesh& sphereMesh)
	{
		InitFloor(shader, cubeMesh, sphereMesh);
		InitObstacles(shader, cubeMesh, sphereMesh);
	}

	// Update
	void Update(float delta){}

	// Render
	void Render(const glm::mat4& viewProjection)
	{
		// render static objects
		for (auto& gameObject : gameObjects)
		{
			gameObject->Render(viewProjection);
		}
	}

private:

	void InitFloor(Shader& shader, CubeMesh& cubeMesh, SphereMesh& sphereMesh)
	{
		size = MathGeom::Vector3(100.f, 0.0001f, 70.0f);
		position = MathGeom::Vector3(0.0f, 0.0f, 10.0f);

		auto gameObject = std::make_shared<GameObject>();
		gameObject->transform.position = position;
		gameObject->transform.scale = size;
		gameObject->SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, MathGeom::Vector4(0.8f, 0.8f, 0.8f, 1.0f)));
		gameObject->SetVisible(true);

		gameObjects.emplace_back(gameObject);
	}

	void InitObstacles(Shader& shader, CubeMesh& cubeMesh, SphereMesh& sphereMesh)
	{
		auto obstacle = std::make_shared<GameObject>();
		obstacle->transform.position = MathGeom::Vector3(40.0f, 0.0f, 15.0f);
		obstacle->transform.scale = MathGeom::Vector3(10.f, 5.0f, 20.f);
		obstacle->SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, MathGeom::Vector4(0.11f, 0.11f, 0.11f, 1.0f)));
		obstacle->SetVisible(true);

		gameObjects.emplace_back(obstacle);
		obstacles.emplace_back(obstacle);

		obstacle = std::make_shared<GameObject>();
		obstacle->transform.position = MathGeom::Vector3(60.0f, 0.0f, 0.0f);
		obstacle->transform.scale = MathGeom::Vector3(20.f, 5.0f, 5.f);
		obstacle->SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, MathGeom::Vector4(0.11f, 0.11f, 0.11f, 1.0f)));
		obstacle->SetVisible(true);

		gameObjects.emplace_back(obstacle);
		obstacles.emplace_back(obstacle);

		obstacle = std::make_shared<GameObject>();
		obstacle->transform.position = MathGeom::Vector3(-50.0f, 0.0f, 45.0f);
		obstacle->transform.rotation = MathGeom::Vector3(0.0f, -45.0f, 0.0f);
		obstacle->transform.scale = MathGeom::Vector3(10.f, 5.0f, 20.f);
		obstacle->SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, MathGeom::Vector4(0.11f, 0.11f, 0.11f, 1.0f)));
		obstacle->SetVisible(true);

		gameObjects.emplace_back(obstacle);
		obstacles.emplace_back(obstacle);

		obstacle = std::make_shared<GameObject>();
		obstacle->transform.position = MathGeom::Vector3(-40.0f, 0.0f, 30.0f);
		obstacle->transform.rotation = MathGeom::Vector3(0.0f, 0.0f, 0.0f);
		obstacle->transform.scale = MathGeom::Vector3(5.f, 5.0f, 15.f);
		obstacle->SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, MathGeom::Vector4(0.11f, 0.11f, 0.11f, 1.0f)));
		obstacle->SetVisible(true);

		gameObjects.emplace_back(obstacle);
		obstacles.emplace_back(obstacle);
	}
};

#endif // !WORLD_MAP_H

