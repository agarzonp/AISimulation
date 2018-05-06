#ifndef PATH_PLANNER_TYPES_H
#define	PATH_PLANNER_TYPES_H

enum class PathPlannerType
{
	A_STAR,
	JUMP_POINT_SEARCH
};

#include "PathPlanner.h"

#include "AStar.h"
#include "JumpPointSearch.h"

#endif // !PATH_PLANNER_TYPES_H

