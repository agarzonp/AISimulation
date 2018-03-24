#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

#include <cassert>
#include <memory>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../Input/Input.h"
#include "../Shaders/Shader.h"

#include "MathGeom.h"
#include "Camera/FreeCamera.h"
#include "Pathfinding/Pathfinder.h"

#include "AIEntity.h"
#include "GameObject.h"

#include "Render/Meshes/CubeMesh.h"
#include "Render/Meshes/SphereMesh.h"
#include "Render/Renderable.h"
#include "Render/SphereRenderable.h"
#include "Render/RenderUtils.h"

#include "Physics/PhysicsEngine.h"

class TestEnvironment : public InputListener
{

public:
	TestEnvironment()
	{
		Init();
	}

	~TestEnvironment()
	{
		Terminate();
	}

	void OnKeyPressed(int key) override 
	{
		switch (key)
		{
		case GLFW_KEY_S:
			worldState.isTimeToSleep = true;
			break;
		case GLFW_KEY_W:
			worldState.isTimeToSleep = false;
		case GLFW_KEY_O:
			worldState.isDoorOpen = !worldState.isDoorOpen;
			break;

		case GLFW_KEY_1:
		{
			pathfinder.debugRenderSearchSpace = !pathfinder.debugRenderSearchSpace;
			break;
		}
		case GLFW_KEY_2:
		{
			pathfinder.debugRenderPath = !pathfinder.debugRenderPath;
			break;
		}
		case GLFW_KEY_TAB:
		{
			static PathScheduler::PathRequestHandler pathRequest;
			if (pathRequest)
			{
				// cancel current request as we are goint to request a new one
				pathRequest.Cancel();
			}

			std::srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

			PathRequestData pathRequestData;
			pathRequestData.start = MathGeom::Vector3(-50 + std::rand()%100, 0.0f, -50 + std::rand() % 100);
			pathRequestData.goal = MathGeom::Vector3(-50 + std::rand() % 100, 0.0f, -50 + std::rand() % 100);
			pathRequestData.onPathRequestResult = [](PathRequestId id, PathRequestResultStatus resultStatus, Path& path)
			{
				printf("PathRequest %d result: %d pathSize: %d\n", id, resultStatus, path.size());
			};

			pathRequest = pathfinder.RequestPath(pathRequestData);
			break;
		}
			
			
		}
	}

	void OnKeyReleased(int key) override { }
	void OnMouseButtonPressed(int button, double x, double y) override { }
	void OnMouseButtonReleased(int button, double x, double y) override { }
	void OnMouseScroll(double xoffset, double yoffset) override { }
	
	void OnMouseMove(double x, double y) { }

	// Update
	void Update(float deltaTime) 
	{
		camera.Update(deltaTime);

		physicsEngine.Update(deltaTime);

		aiEntity.Update(worldState);

		pathfinder.Update();
	}

	// Render
	void Render() 
	{
		// use the shader
		shader.Use();

		const glm::mat4& viewProjection = camera.ViewProjectionMatrix();
		for (auto& gameObject : gameObjects)
		{
			gameObject.Render(viewProjection);
		}

		pathfinder.DebugRender(viewProjection);
	}
	
protected:

	void Init()
	{
		// init cube mesh
		cubeMesh.Init();

		// load shader
		shader.Load("assets/Shaders/basic.vert", "assets/Shaders/basic.frag");

		// Init render utils
		RenderUtils::InitCubeRenderable(shader, &cubeMesh);

		// init game objects
		InitGameObjects();

		// init camera
		camera.Init(glm::vec3(0.0f, 150.0f, -40.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 1024.0f / 768.0f, 0.1f, 1000000.0f);

		// init physics
		InitPhysics();

		// init pathfinding system
		InitPathfinder();
	}
		
	void InitGameObjects()
	{
		GameObject gameObject;

		// floor
		gameObject.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
		gameObject.transform.scale = glm::vec3(50.0f, 0.0001f, 50.f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f)));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		// walls
		gameObject.transform.position = glm::vec3(50.0f, 5.0f, 0.0f);
		gameObject.transform.scale = glm::vec3(0.5f, 5.0f, 50.f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(0.11f, 0.11f, 0.11f, 1.0f)));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		gameObject.transform.position = glm::vec3(-50.0f, 5.0f, 0.0f);
		gameObject.transform.scale = glm::vec3(0.5f, 5.0f, 50.f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(0.11f, 0.11f, 0.11f, 1.0f)));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		gameObject.transform.position = glm::vec3(0.0f, 5.0f, 50.0f);
		gameObject.transform.scale = glm::vec3(50.0f, 5.0f, 0.5f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(0.11f, 0.11f, 0.11f, 1.0f)));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		gameObject.transform.position = glm::vec3(0.0f, 5.0f, -50.0f);
		gameObject.transform.scale = glm::vec3(50.0f, 5.0f, 0.5f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(0.11f, 0.11f, 0.11f, 1.0f)));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		// green sphere

		gameObject.transform.position = glm::vec3(-20.0f, 5.5f, 40.0f);
		gameObject.transform.rotation = glm::vec3(0.5f, 0.5f, 0.0f);
		gameObject.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		gameObject.SetRenderable(std::shared_ptr<Renderable>(static_cast<Renderable*>(new SphereRenderable(3.0f, &sphereMesh, shader, glm::vec4(0.0f, 0.3f, 0.0f, 1.0f)))));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		gameObject.transform.position = glm::vec3(25.0f, 5.5f, 40.0f);
		gameObject.transform.rotation = glm::vec3(0.5f, 0.5f, 0.0f);
		gameObject.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		gameObject.SetRenderable(std::shared_ptr<Renderable>(static_cast<Renderable*>(new SphereRenderable(5.0f, &sphereMesh, shader, glm::vec4(0.0f, 0.3f, 0.0f, 1.0f)))));
		gameObject.SetVisible(true);

		gameObjects.emplace_back(gameObject);

		// red cube

		gameObject.transform.position = glm::vec3(25.0f, 0.0f, 40.0f);
		gameObject.transform.rotation = glm::vec3(0.5f, 0.5f, 0.0f);
		gameObject.transform.scale = glm::vec3(2.5f, 2.5f, 2.5f);
		gameObject.SetRenderable(std::make_shared<Renderable>(&cubeMesh, shader, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));
		gameObject.SetVisible(false);

		gameObjects.emplace_back(gameObject);
	}

	void InitPhysics()
	{
		physicsEngine.Init();

		// green sphere
		PhysicObjectDesc desc5;
		desc5.type = PhysicObjectType::PARTICLE;
		desc5.mass = 10.0f;
		desc5.velocity = MathGeom::Vector3(2.0f, 0.0f, 0.0f);
		desc5.acceleration = MathGeom::Vector3(10.0f, 0.0f, 0.0f);
		desc5.colliderDesc = std::make_unique<SphereColliderDesc>(3.0f, gameObjects[5].transform);
		desc5.isAffectedByGravity = false;

		physicsEngine.AddPhysics(gameObjects[5], desc5);

		PhysicObjectDesc desc6;
		desc6.type = PhysicObjectType::PARTICLE;
		desc6.mass = 10.0f;
		desc6.velocity = MathGeom::Vector3(-2.0f, 0.0f, 0.0f);
		desc6.acceleration = MathGeom::Vector3(-10.0f, 0.0f, 0.0f);
		desc6.colliderDesc = std::make_unique<SphereColliderDesc>(5.0f, gameObjects[6].transform);
		desc6.isAffectedByGravity = false;

		physicsEngine.AddPhysics(gameObjects[6], desc6);
		
		// red cube
		PhysicObjectDesc desc7;
		desc7.type = PhysicObjectType::PARTICLE;
		desc7.mass = 10.0f;
		desc7.velocity = MathGeom::Vector3(-2.0f, 0.0f, 0.0f);
		desc7.acceleration = MathGeom::Vector3(-10.0f, 0.0f, 0.0f);

		//physicsEngine.AddPhysics(gameObjects[7], desc7);
	}

	void InitPathfinder()
	{
		PathfinderData pathfinderData;
		pathfinderData.pathPlannerData.type = PathPlannerType::A_STAR;
		pathfinderData.searchSpaceData.searchSpaceType = SearchSpaceType::OCTILE_GRID;
		pathfinderData.searchSpaceData.anchorPosition = MathGeom::Vector3(-50.0f, 0.0f, -50.0f);
		pathfinderData.searchSpaceData.worldSize = MathGeom::Vector3(100.0f, 100.0f, 100.0f);
		pathfinderData.gridCellSize = 10.0f;
		pathfinder.Init(pathfinderData);
	}

	void Terminate()
	{
	}

private:
	
	// shader
	Shader shader;

	// cube mesh
	CubeMesh cubeMesh;

	// sphere mesh
	SphereMesh sphereMesh;

	// Camera
	FreeCamera camera;

	// game objects
	std::vector<GameObject> gameObjects;

	// Physics engine
	PhysicsEngine physicsEngine;

	// Pathfinder
	Pathfinder pathfinder;

	// World state
	WorldState worldState;

	// AI entity
	AIEntity aiEntity;
};

#endif