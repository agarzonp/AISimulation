#ifndef PATHFINDER_DEBUG_RENDER_FLAGS_H
#define PATHFINDER_DEBUG_RENDER_FLAGS_H

struct PathfinderDebugRenderFlags
{
	bool enabled{ false };
	bool startGoalWorldPosition{ false };
	bool startGoalSearchSpace{ false };
	bool pathPlannerPath{ false };
	bool finalPath{ false };
	bool searchSpace{ false };
};

#endif // !PATHFINDER_DEBUG_RENDER_FLAGS_H

