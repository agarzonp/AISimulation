#ifndef PATH_SCHEDULER_H
#define PATH_SCHEDULER_H

#include <list>
#include <map>

#include "PathRequest.h"
#include "PathRequestQueue.h"

#include "../SearchSpace/SearchSpace.h"

class PathScheduler
{
	// path requests
	std::map<PathRequestId, PathRequest> requests;

	// request queue
	PathRequestQueue requestQueue;

	// request count
	size_t requestCount{ 0 };

	// search space
	std::shared_ptr<SearchSpace> searchSpace;

	// open and closed list
	std::list<PathNode*> open;
	std::list<PathNode*> close;

	// start/goal nodes
	PathNode* start{ nullptr };
	PathNode* goal{ nullptr };

public:

	// Set search space
	void SetSearchSpace(std::shared_ptr<SearchSpace> searchSpace)
	{
		this->searchSpace = searchSpace;
	}

	// Handler for the path request
	class PathRequestHandler
	{
		// id
		PathRequestId id { 0 };
		PathScheduler* pathScheduler { nullptr };

		public:

			// Constructors
			PathRequestHandler() = default;
			PathRequestHandler(PathRequestId id_, PathScheduler* pathfinder_) : id(id_), pathScheduler(pathfinder_) {}

			// Desctructor
			//~PathRequestHandler() { Cancel(); } // FIXME: Use move semantics!

			// operator bool
			operator bool() const { return id > 0 && pathScheduler; }

			// Cancel
			void Cancel()
			{
				if (pathScheduler)
				{
					pathScheduler->CancelRequest(id);
					pathScheduler = nullptr;
					id = 0;
				}
			}
	};

	// Request path
	PathRequestHandler RequestPath(const PathRequestData& requestData)
	{
		// increase request count
		requestCount++;

		// register the request
		requests[requestCount] = { requestCount, requestData, PathRequest::State::INVALID };
		auto& request = requests[requestCount];

		// enqueue
		if (requestQueue.Enqueue(request))
		{
			request.state = PathRequest::State::QUEUED;
		}

		// return a handler for the registered request
		return PathRequestHandler(request.id, this);
	}

	// Update
	void Update()
	{
		// find paths
		FindPaths();
	}

	// Debug Render
	void DebugRender(const MathGeom::Matrix4& viewProjection)
	{
		if (start && goal)
		{
			Transform transform;
			transform.position = start->position;
			RenderUtils::RenderCube(viewProjection, transform, 0xFF0000);

			transform.position = goal->position;
			RenderUtils::RenderCube(viewProjection, transform, 0xFF0000);
		}
	}

protected:

	// Cancel request
	void CancelRequest(PathRequestId requestId)
	{
		requestQueue.Remove(requestId);
		requests.erase(requestId);
	}

private:

	// Find paths
	void FindPaths()
	{
		auto request = requestQueue.GetRequest();
		if (!request)
		{
			// no requests left
			return;
		}

		switch (request->state)
		{
		case PathRequest::State::QUEUED:
		case PathRequest::State::INTERRUPTED:
			StartSearch(*request);
			break;
		case PathRequest::State::RUNNING:
			ResumeSearch(*request);
			break;
		case PathRequest::State::COMPLETED:
		case PathRequest::State::INVALID:
		
			assert(false);
			break;
		default:
			assert(false);
			break;
		}
	}

	// Start search
	void StartSearch(PathRequest& request)
	{
		assert(request.state == PathRequest::State::QUEUED || request.state == PathRequest::State::INTERRUPTED);

		// running request
		request.state = PathRequest::State::RUNNING;

		// Localise start/goal positions
		start = searchSpace->Localise(request.data.start);
		goal = searchSpace->Localise(request.data.goal);

		// Validate request
		if (Validate(request, start, goal))
		{
			// reset
			Reset();

			// init open list
			open.push_back(start);

			// search path
			SearchPath(request);
		}		
	}

	// Resume search
	void ResumeSearch(PathRequest& request)
	{
		SearchPath(request);
	}
	
private:

	// Validate request
	bool Validate(PathRequest& request, PathNode* start, PathNode* goal)
	{
		assert(start && goal);
		if (!start)
		{
			TerminateRequest(request, PathRequestResultStatus::PathNotFound_StartNotLocalised, Path());
			return false;
		}

		if (!goal)
		{
			TerminateRequest(request, PathRequestResultStatus::PathNotFound_GoalNotLocalised, Path());
			return false;
		}

		if (start->type == PathNodeType::BLOCKED)
		{
			TerminateRequest(request, PathRequestResultStatus::PathNotFound_StartBlocked, Path());
			return false;
		}

		if (goal->type == PathNodeType::BLOCKED)
		{
			TerminateRequest(request, PathRequestResultStatus::PathNotFound_GoalBlocked, Path());
			return false;
		}

		return true;
	}

	// Reset
	void Reset()
	{
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
	}

	// Terminate request
	void TerminateRequest(PathRequest& request, PathRequestResultStatus resultStatus, Path& path)
	{
		// mark as completed
		request.state = PathRequest::State::COMPLETED;

		// notify the result
		request.data.onPathRequestResult(request.id, resultStatus, path);

		// cancel current request
		CancelRequest(request.id);
	}

private:

	// Search path
	void SearchPath(PathRequest& request)
	{
		Path path;
		if (SearchPath(path))
		{
			PathRequestResultStatus resultStatus = path.size() ? PathRequestResultStatus::PathFound : PathRequestResultStatus::PathNotFound;
			TerminateRequest(request, resultStatus, path);
		}
	}

	// Search path
	bool SearchPath(Path& path)
	{
		bool searchCompleted = false;
		bool pathFound = false;

		while (open.size() > 0)
		{
			// get cheapest from open
			auto current = PopCheapestFromOpen();
			if (current == goal)
			{
				// path found!
				pathFound = true;
				searchCompleted = true;
				close.push_back(current);			
				break;
			}

			// Evaluate
			Evaluate(current);
		}

		searchCompleted = true;

		if (pathFound)
		{
			SetPath(path);
		}

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
			if (!neighbour)
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

	void SetPath(Path& path)
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

	float DistanceSq(PathNode* nodeA, PathNode* nodeB)
	{
		return MathGeom::DistanceSq(nodeA->position, nodeB->position);
	}


};

#endif // !PATHFINDER_H

