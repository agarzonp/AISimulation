#ifndef PATHFINDER_H
#define PATHFINDER_H

#include "SearchSpace/NavGrid.h"
#include "PathScheduler/PathScheduler.h"

struct PathfindingSystemData
{
	SearchSpaceData searchSpaceData;
	float gridCellSize{ 1.0f };
};

class Pathfinder
{
	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// path scheduler
	PathScheduler pathScheduler;

public:
	
	// Init
	void Init(const PathfindingSystemData& data)
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

		// set pathfinder search space
		pathScheduler.SetSearchSpace(searchSpace);
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
			pathScheduler.DebugRender(viewProjection);
		}

		if (searchSpace && debugRenderSearchSpace)
		{
			searchSpace->DebugRender(viewProjection);
		}
	}

private:

};

#endif // !PATHFINDING_SYSTEM_H
