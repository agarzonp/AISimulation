#ifndef PATHFINDING_SYSTEM_H
#define PATHFINDING_SYSTEM_H

#include "NavGrid.h"
#include "Pathfinder.h"

struct PathfindingSystemData
{
	SearchSpaceData searchSpaceData;
	float gridCellSize{ 1.0f };
};

class PathfindingSystem
{
	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// pathfinder
	Pathfinder pathfinder;

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
		pathfinder.SetSearchSpace(searchSpace);
	}

	// Request path
	Pathfinder::PathRequestHandler RequestPath(const PathRequestData& requestData)
	{
		return pathfinder.RequestPath(requestData);
	}

	// Update
	void Update()
	{
		pathfinder.Update();
	}

	// Debug render
	void DebugRender()
	{
		// TO-DO
	}

private:

};

#endif // !PATHFINDING_SYSTEM_H
