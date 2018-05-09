#ifndef PATH_PLANNER_H
#define	PATH_PLANNER_H

#include "../PathNode.h"
#include "../SearchSpace/SearchSpace.h"
#include "../PathfinderDebugRenderFlags.h"

// number of revolutions that the algorithm can perform
static const int DEFAULT_MAX_REVOLUTIONS = 200;

// Path planner data
struct PathPlannerData
{
	PathPlannerType type;
	int maxRevolutions = DEFAULT_MAX_REVOLUTIONS;
};

// Abstrac path planner
class PathPlanner
{

protected:

	bool searchCompleted{ false };
	bool pathFound{ false };

	int maxRevolutions{ DEFAULT_MAX_REVOLUTIONS };

	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// planner path (for debugging purpose)
	std::vector<PathNode*> plannerPath;

public:

	// Constructors
	PathPlanner(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace_)
		: maxRevolutions(data.maxRevolutions)
		, searchSpace(searchSpace_)
	{
	}

	// Start search
	virtual bool StartSearch(PathNode* start, PathNode* goal) = 0;

	// Resume search
	virtual bool ResumeSearch() = 0;

	// Get path
	virtual void GetPath(Path& path) = 0;

	// Reset
	virtual void Reset() = 0;
	
	// Debug render
	virtual void DebugRender(const MathGeom::Matrix4& viewProjection, const PathfinderDebugRenderFlags& render) = 0;
};

#endif // !PATH_PLANNER_H

