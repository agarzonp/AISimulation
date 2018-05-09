#ifndef OPEN_CLOSE_PATH_PLANNER_H
#define OPEN_CLOSE_PATH_PLANNER_H

#include <list>

#include "PathPlanner.h"

class OpenClosePathPlanner : public PathPlanner
{
protected:

	// open and closed list
	std::list<PathNode*> open;
	std::list<PathNode*> close;

	// start/goal nodes
	PathNode* start{ nullptr };
	PathNode* goal{ nullptr };

private:

	// Expand
	virtual void Expand(PathNode* current) = 0;

public:
	// Constructors
	OpenClosePathPlanner(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace)
		: PathPlanner(data, searchSpace)
	{
	}

	// Start search
	bool StartSearch(PathNode* start, PathNode* goal) final
	{
		// reset
		Reset();

		// set start/goal
		this->start = start;
		this->goal = goal;

		// init open list
		open.push_back(start);

		// search
		return Search();
	}

	// Resume search
	bool ResumeSearch() final
	{
		// keep searching
		return Search();
	}

	// Get path
	void GetPath(Path& path) final
	{
		if (pathFound)
		{
			// go backwards to get the path

			auto node = close.back();
			PathNode* current = node;
			PathNode* parent = node->parent;
			
			plannerPath.clear();
			plannerPath.push_back(current);

			path.push_back(node->position);

			while (parent)
			{
				plannerPath.push_back(parent);

				// make sure that adjacency between current node and parent node is valid
				node = searchSpace->GetValidatedPathNode(current, parent);
				
				path.push_back(node->position);

				current = parent;
				parent = parent->parent;
			}

			// reverse the path
			std::reverse(path.begin(), path.end());
			std::reverse(plannerPath.begin(), plannerPath.end());
		}
	}

	// Debug Render
	void DebugRender(const MathGeom::Matrix4& viewProjection, const PathfinderDebugRenderFlags& render) final
	{
		if (start && goal && (render.pathPlannerPath || render.finalPath))
		{
			Path path;
			GetPath(path);

			if (render.pathPlannerPath)
			{
				for (auto& node : plannerPath)
				{
					Transform transform;
					transform.position = node->position;
					RenderUtils::RenderCube(viewProjection, transform, 0xFF0000);
				}
			}

			if (render.finalPath)
			{
				for (auto& p : path)
				{
					Transform transform;
					transform.position = p;
					RenderUtils::RenderCube(viewProjection, transform, 0x00FF00);
				}
			}
			
		}
	}

	// Reset
	void Reset() override
	{
		searchCompleted = false;
		pathFound = false;

		// clear lists
		auto ClearList = [this](std::list<PathNode*>& list)
		{
			// reset nodes 
			for (auto& node : list)
			{
				node->fCost = 0.0f;
				node->gCost = 0.0f;
				node->hCost = 0.0f;
				node->parent = nullptr;
				node->isForced = false;
			}

			list.clear();
		};

		ClearList(open);
		ClearList(close);

		start = nullptr;
		goal = nullptr;
	}
	
protected:

	// Update cost
	void UpdateCost(PathNode* current, PathNode* neighbour)
	{
		if (std::find(close.begin(), close.end(), neighbour) != close.end())
		{
			// ignore if it is in close
			return;
		}

		if (std::find(open.begin(), open.end(), neighbour) != open.end())
		{
			// check if this path is better
			float gCost = current->gCost + DistanceEuclidean(current, neighbour);
			if (gCost < neighbour->gCost)
			{
				// update neighbour
				neighbour->parent = current;
				neighbour->gCost = gCost;
				neighbour->fCost = neighbour->hCost + gCost;
			}
		}
		else
		{
			// update neighbour
			neighbour->parent = current;
			neighbour->hCost = DistanceManhattan(neighbour, goal);
			neighbour->gCost = current->gCost + DistanceEuclidean(current, neighbour);
			neighbour->fCost = neighbour->hCost + neighbour->gCost;

			// push in open
			open.push_back(neighbour);
		}
	}

private:

	// Search
	bool Search()
	{
		int revolution = 0;
		while (open.size() > 0)
		{
			// get cheapest from open
			auto current = PopCheapestFromOpen();
			if (current == goal)
			{
				// path found!
				pathFound = true;
				close.push_back(current);
				break;
			}

			// Expand
			Expand(current);

			// check search allowance
			if (revolution++ >= maxRevolutions)
			{
				// allowance surpassed so exit and continue next frame
				break;
			}
		}

		searchCompleted = (pathFound || open.size() == 0);

		return searchCompleted;
	}

	// Pop cheapest from open
	PathNode* PopCheapestFromOpen()
	{
		PathNode* cheapest = open.front();
		auto cheapestIterator = open.begin();
		for (auto nodeIterator = open.begin(); nodeIterator != open.end(); ++nodeIterator)
		{
			PathNode* node = *nodeIterator;
			if (node->fCost < cheapest->fCost)
			{
				cheapestIterator = nodeIterator;
				cheapest = *cheapestIterator;
			}
			else if (node->fCost == cheapest->fCost && node->hCost < cheapest->hCost)
			{
				cheapestIterator = nodeIterator;
				cheapest = *cheapestIterator;
			}
		}

		open.erase(cheapestIterator);

		return cheapest;
	}

	// Distance Euclidean
	float DistanceEuclidean(PathNode* nodeA, PathNode* nodeB)
	{
		return MathGeom::Distance(nodeA->position, nodeB->position);
	}	

	// Distance Manhattan
	float DistanceManhattan(PathNode* nodeA, PathNode* nodeB)
	{
		float dx = fabsf(nodeA->position.x - nodeB->position.x);
		float dy = fabsf(nodeA->position.y - nodeB->position.y);
		float dz = fabsf(nodeA->position.z - nodeB->position.z);

		return dx + dy + dz;
	}
};

#endif // !OPEN_CLOSE_PATH_PLANNER_H

