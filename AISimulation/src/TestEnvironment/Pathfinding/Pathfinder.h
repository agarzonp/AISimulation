#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

// Path
using Path = std::vector<MathGeom::Vector3>;

#include "SearchSpace/SearchSpaceTypes.h"
#include "PathPlanner/PathPlannerTypes.h"
#include "PathRequestScheduler/PathRequestScheduler.h"

// Pathfinder data
struct PathfinderData
{
	SearchSpaceData searchSpaceData;
	PathPlannerData pathPlannerData;
};

class Pathfinder
{
	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// path planner
	std::shared_ptr<PathPlanner> pathPlanner;

	// path request scheduler
	PathRequestScheduler pathRequestScheduler;

public:
	
	// Init
	void Init(const PathfinderData& data)
	{
		// set search space
		SetSearchSpace(data.searchSpaceData);

		// set planner
		SetPathPlanner(data.pathPlannerData);
	}

	// set search space
	void SetSearchSpace(const SearchSpaceData& searchSpaceData)
	{
		switch (searchSpaceData.searchSpaceType)
		{
		case SearchSpaceType::OCTILE_GRID:
			searchSpace = std::make_shared<NavGrid>(searchSpaceData);
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

		// set search space to the scheduler
		pathRequestScheduler.SetSearchSpace(searchSpace);
	}

	// Set planner type
	void SetPathPlanner(const PathPlannerData& pathPlannerData)
	{
		// create planner
		switch (pathPlannerData.type)
		{
		case PathPlannerType::A_STAR:
			pathPlanner = std::make_shared<AStar>(pathPlannerData);
			break;
		case PathPlannerType::JUMP_POINT_SEARCH:
			pathPlanner = std::make_shared<JumpPointSearch>(pathPlannerData);
			break;
		default:
			assert(false);
			break;
		}

		// set planner to the scheduler
		pathRequestScheduler.SetPathPlanner(pathPlanner);
	}

	// Request path
	PathRequestId RequestPath(const PathRequestData& requestData)
	{
		// add the request to the scheduler
		return pathRequestScheduler.AddRequest(requestData);
	}

	// Cancel request
	void CancelRequest(PathRequestId requestId)
	{
		return pathRequestScheduler.CancelRequest(requestId);
	}

	// Update
	void Update()
	{
		pathRequestScheduler.Update();
	}

	// Debug render
	bool debugRenderSearchSpace{ false };
	bool debugRenderPath{ false };
	void DebugRender(const MathGeom::Matrix4& viewProjection)
	{
		if (debugRenderPath)
		{
			pathRequestScheduler.DebugRender(viewProjection);
		}

		if (searchSpace && debugRenderSearchSpace)
		{
			searchSpace->DebugRender(viewProjection);
		}
	}

private:

};

#endif // !PATHFINDING_SYSTEM_H
