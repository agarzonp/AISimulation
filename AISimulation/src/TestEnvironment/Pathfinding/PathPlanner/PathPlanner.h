#ifndef PATH_PLANNER_H
#define	PATH_PLANNER_H

#include "../PathNode.h"

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

public:

	// Constructors
	PathPlanner(const PathPlannerData& data): maxRevolutions(data.maxRevolutions){}

	// Start search
	virtual bool StartSearch(PathNode* start, PathNode* goal) = 0;

	// Resume search
	virtual bool ResumeSearch() = 0;

	// Get path
	virtual void GetPath(Path& path) = 0;

	// Reset
	virtual void Reset() = 0;
	
	// Debug render
	virtual void DebugRender(const MathGeom::Matrix4& viewProjection) = 0;
};

#endif // !PATH_PLANNER_H

