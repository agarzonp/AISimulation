#ifndef A_STAR_H
#define A_STAR_H

#include <list>

#include "PathPlanner.h"

class AStar : public PathPlanner
{
	// open and closed list
	std::list<PathNode*> open;
	std::list<PathNode*> close;

	// start/goal nodes
	PathNode* start{ nullptr };
	PathNode* goal{ nullptr };

public:

	// Constructors
	AStar(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace) 
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
			while (node)
			{
				path.push_back(node->position);
				node = node->parent;
			}

			// reverse the path
			std::reverse(path.begin(), path.end());
		}
	}

	// Debug Render
	void DebugRender(const MathGeom::Matrix4& viewProjection) final
	{
		if (start && goal)
		{
			Transform transform;
			transform.position = start->position;
			RenderUtils::RenderCube(viewProjection, transform, 0x00FF00);

			transform.position = goal->position;
			RenderUtils::RenderCube(viewProjection, transform, 0x00FF00);

			Path path;
			GetPath(path);

			for (auto& p : path)
			{
				Transform transform;
				transform.position = p;
				RenderUtils::RenderCube(viewProjection, transform, 0xFF0000);
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
			}

			list.clear();
		};

		ClearList(open);
		ClearList(close);

		start = nullptr;
		goal = nullptr;
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

			// Evaluate
			Evaluate(current);

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

	// Evaluate
	void Evaluate(PathNode* current)
	{
		for (auto neighbour : current->neighbours)
		{
			if (!searchSpace->IsValidAdjacency(current, neighbour))
				continue;

			if (std::find(close.begin(), close.end(), neighbour) != close.end())
			{
				// ignore if it is in close
				continue;
			}

			if (std::find(open.begin(), open.end(), neighbour) != open.end())
			{
				// check if this path is better
				float gCost = current->gCost + DistanceSq(current, neighbour);
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
				neighbour->hCost = DistanceSq(neighbour, goal);
				neighbour->gCost = current->gCost + DistanceSq(current, neighbour);
				neighbour->fCost = neighbour->hCost + neighbour->gCost;

				// push in open
				open.push_back(neighbour);
			}
		}

		// push current in close
		close.push_back(current);
	}

	float DistanceSq(PathNode* nodeA, PathNode* nodeB)
	{
		return MathGeom::DistanceSq(nodeA->position, nodeB->position);
	}
};

#endif // !A_STAR_H

