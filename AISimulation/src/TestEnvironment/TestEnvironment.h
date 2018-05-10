#ifndef TEST_ENVIRONMENT_H
#define TEST_ENVIRONMENT_H

#include <cassert>
#include <memory>
#include <list>

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

#include "Gameplay/WorldMap.h"

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
			pathfinder.debugRenderFlags.enabled = !pathfinder.debugRenderFlags.enabled;
			break;
		}
		case GLFW_KEY_2:
		{
			pathfinder.debugRenderFlags.searchSpace = !pathfinder.debugRenderFlags.searchSpace;
			break;
		}
		case GLFW_KEY_3:
		{
			pathfinder.debugRenderFlags.startGoalWorldPosition = !pathfinder.debugRenderFlags.startGoalWorldPosition;
			break;
		}
		case GLFW_KEY_4:
		{
			pathfinder.debugRenderFlags.startGoalSearchSpace = !pathfinder.debugRenderFlags.startGoalSearchSpace;
			break;
		}
		case GLFW_KEY_5:
		{
			pathfinder.debugRenderFlags.pathPlannerPath = !pathfinder.debugRenderFlags.pathPlannerPath;
			break;
		}
		case GLFW_KEY_6:
		{
			pathfinder.debugRenderFlags.finalPath = !pathfinder.debugRenderFlags.finalPath;
			break;
		}
		case GLFW_KEY_TAB:
		{
			static PathRequestId pathRequestId;

			// cancel current request as we are goint to request a new one
			pathfinder.CancelRequest(pathRequestId);

			std::srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());

			PathRequestData pathRequestData;
			pathRequestData.start = MathGeom::Vector3(-50 + std::rand()%100, 0.0f, -50 + std::rand() % 100);
			pathRequestData.goal = MathGeom::Vector3(-50 + std::rand() % 100, 0.0f, -50 + std::rand() % 100);
			//pathRequestData.start = MathGeom::Vector3(-1, 0.0f, 18);
			//pathRequestData.goal = MathGeom::Vector3(38, 0.0f, 42);

			pathRequestData.onPathRequestResult = [pathRequestData](PathRequestId id, PathRequestResultStatus resultStatus, Path& path)
			{
				printf("PathRequest %d ([%d, %d] - [%d, %d]) result: %d pathSize: %d\n", id, (int)pathRequestData.start.x, (int)pathRequestData.start.z, (int)pathRequestData.goal.x, (int)pathRequestData.goal.z, resultStatus, path.size());
			};

			pathRequestId = pathfinder.RequestPath(pathRequestData);
			break;
		}

		case GLFW_KEY_J:
		{
			static bool jps = false;
			jps = !jps;

			PathPlannerData plannerData;
			plannerData.type = jps ? PathPlannerType::JUMP_POINT_SEARCH : PathPlannerType::A_STAR;
			pathfinder.SetPathPlanner(plannerData);
		}

		case GLFW_KEY_M:
		{
			PathRequestData pathRequestData;
			pathRequestData.start = aiEntity.transform.position;
			pathRequestData.goal = MathGeom::Vector3(-46, 0.0f, 22);
			
			pathRequestData.onPathRequestResult = [pathRequestData, this](PathRequestId id, PathRequestResultStatus resultStatus, Path& path)
			{
				printf("PathRequest %d ([%d, %d] - [%d, %d]) result: %d pathSize: %d\n", id, (int)pathRequestData.start.x, (int)pathRequestData.start.z, (int)pathRequestData.goal.x, (int)pathRequestData.goal.z, resultStatus, path.size());
				this->aiEntity.SetPath(path);
			};

			pathfinder.RequestPath(pathRequestData);

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
		// update camera
		camera.Update(deltaTime);

		// update world map
		worldMap.Update(deltaTime);
		
		// update AI
		aiEntity.Update(worldState);

		// update pathfinfer
		pathfinder.Update();

		// update physics
		physicsEngine.Update(deltaTime);
	}

	// Render
	void Render() 
	{
		// use the shader
		shader.Use();

		const glm::mat4& viewProjection = camera.ViewProjectionMatrix();

		// render world map
		worldMap.Render(viewProjection);

		// render ai entity
		aiEntity.Render(viewProjection);

		// render physics
		physicsEngine.DebugRender(viewProjection);

		// debug render pathfinder
		pathfinder.DebugRender(viewProjection);
	}
	
protected:

	void Init()
	{
		// init cube mesh
		cubeMesh.Init();
		sphereMesh.Init();

		// load shader
		shader.Load("assets/Shaders/basic.vert", "assets/Shaders/basic.frag");

		RenderUtils::InitCubeRenderable(shader, &cubeMesh);
		RenderUtils::InitSphereRenderable(shader, &sphereMesh);

		// init camera
		camera.Init(glm::vec3(0.0f, 190.0f, -30.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 1024.0f / 768.0f, 0.1f, 1000000.0f);

		// init physics engine
		physicsEngine.Init();

		// init world map
		worldMap.Init(shader, cubeMesh, sphereMesh);

		// init ai entities
		InitAIEntities();

		// init pathfinding system
		InitPathfinder();
	}

	void InitAIEntities()
	{
		aiEntity.transform.position = glm::vec3(25.0f, 0.0f, 40.0f);
		aiEntity.transform.rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		aiEntity.transform.scale = glm::vec3(3.0f, 3.0f, 3.0f);
		float sphereRadius = aiEntity.transform.scale.x;
		aiEntity.SetRenderable(std::shared_ptr<Renderable>(static_cast<Renderable*>(new SphereRenderable(sphereRadius, &sphereMesh, shader, glm::vec4(0.0f, 0.3f, 0.0f, 1.0f)))));
		aiEntity.SetRenderable(std::shared_ptr<Renderable>(static_cast<Renderable*>(new SphereRenderable(sphereRadius, &sphereMesh, shader, glm::vec4(0.0f, 0.3f, 0.0f, 1.0f)))));
		aiEntity.SetVisible(true);

		PhysicObjectDesc physicObject;
		physicObject.type = PhysicObjectType::PARTICLE;
		physicObject.mass = 10.0f;
		physicObject.colliderDesc = std::make_unique<SphereColliderDesc>(aiEntity.transform);
		physicObject.isAffectedByGravity = false;
		physicObject.isStationary = true;

		physicsEngine.AddPhysics(aiEntity, physicObject);
	}

	void InitPathfinder()
	{
		PathfinderData pathfinderData;
		pathfinderData.pathPlannerData.type = PathPlannerType::A_STAR;
		pathfinderData.searchSpaceData.searchSpaceType = SearchSpaceType::OCTILE_GRID;
		pathfinderData.searchSpaceData.anchorPosition = worldMap.Position() - worldMap.Size() * 0.5f;
		pathfinderData.searchSpaceData.worldSize = worldMap.Size();
		pathfinderData.searchSpaceData.gridCellSize = 10.0f;
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

	// Physics engine
	PhysicsEngine physicsEngine;

	// Pathfinder
	Pathfinder pathfinder;

	// World state
	WorldState worldState;

	// AI entity
	AIEntity aiEntity;

	// WorldMap
	WorldMap worldMap;

};

#endif