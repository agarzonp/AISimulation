#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

// Path
using Path = std::vector<MathGeom::Vector3>;

#include "SearchSpace/SearchSpaceTypes.h"
#include "PathPlanner/PathPlannerTypes.h"
#include "PathScheduler/PathScheduler.h"

// Pathfinder data
struct PathfinderData
{
	SearchSpaceData searchSpaceData;
	PathPlannerType pathPlannerType;
	float gridCellSize{ 1.0f };
};

class Pathfinder
{
	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// path planner
	std::shared_ptr<PathPlanner> pathPlanner;

	// path scheduler
	PathScheduler pathScheduler;

public:
	
	// Init
	void Init(const PathfinderData& data)
	{
		// create search space
		switch (data.searchSpaceData.searchSpaceType)
		{
			case SearchSpaceType::OCTILE_GRID:
				searchSpace = std::make_shared<NavGrid>(data.searchSpaceData, data.gridCellSize);
				break;
			default:
				assert(false);
				break;
		}

		// build search space
		if (searchSpace)
		{
			searchSpace->Build();
		}

		// create planner
		switch (data.pathPlannerType)
		{
		case PathPlannerType::A_STAR:
			pathPlanner = std::make_shared<AStar>();
			break;
		default:
			assert(false);
			break;
		}

		// init scheduler
		pathScheduler.Init(searchSpace, pathPlanner);
	}

	// Request path
	PathScheduler::PathRequestHandler RequestPath(const PathRequestData& requestData)
	{
		return pathScheduler.RequestPath(requestData);
	}

	// Update
	void Update()
	{
		pathScheduler.Update();
	}

	// Debug render
	bool debugRenderSearchSpace{ false };
	bool debugRenderPath{ false };
	void DebugRender(const MathGeom::Matrix4& viewProjection)
	{
		if (debugRenderPath)
		{
			pathPlanner->DebugRender(viewProjection);
		}

		if (searchSpace && debugRenderSearchSpace)
		{
			searchSpace->DebugRender(viewProjection);
		}
	}

private:

};

#endif // !PATHFINDING_SYSTEM_H
