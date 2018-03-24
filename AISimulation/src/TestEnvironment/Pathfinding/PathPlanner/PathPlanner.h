#ifndef PATH_PLANNER_H
#define	PATH_PLANNER_H

#include "../PathNode.h"

class PathPlanner
{
protected:

	bool searchCompleted{ false };
	bool pathFound{ false };

public:

	// Start search
	virtual bool StartSearch(PathNode* start, PathNode* goal) = 0;

	// Resume search
	virtual bool ResumeSearch() = 0;

	// Get path
	virtual void GetPath(Path& path) = 0;

	// Debug render
	virtual void DebugRender(const MathGeom::Matrix4& viewProjection) = 0;
};

#endif // !PATH_PLANNER_H

