#ifndef PATH_SCHEDULER_H
#define PATH_SCHEDULER_H

#include <map>

#include "PathRequest.h"
#include "PathRequestQueue.h"

#include "../SearchSpace/SearchSpace.h"
#include "../PathPlanner/PathPlanner.h"

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

	// path planner
	std::shared_ptr<PathPlanner> pathPlanner;

public:

	// Init
	void Init(std::shared_ptr<SearchSpace> searchSpace, std::shared_ptr<PathPlanner> pathPlanner)
	{
		this->searchSpace = searchSpace;
		this->pathPlanner = pathPlanner;
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
			PathRequestHandler(const PathRequestHandler& other) = delete;
			PathRequestHandler(PathRequestHandler&& other) { *this = std::move(other); }
			
			// Desctructor
			~PathRequestHandler() { Cancel(); }

			// Assignment operators
			PathRequestHandler& operator=(const PathRequestHandler& other) = delete;
			PathRequestHandler& operator=(PathRequestHandler&& other)
			{
				if (this != &other)
				{
					this->id = other.id;
					this->pathScheduler = other.pathScheduler;
					
					other.id = 0;
					other.pathScheduler = nullptr;
				}
			
				return *this;
			}

			// operator bool
			operator bool() const { return id > 0 && pathScheduler && pathScheduler->IsRequestEnqueued(id); }

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

protected:

	// Is request enqueued
	bool IsRequestEnqueued(PathRequestId requestId)
	{
		return requests.find(requestId) != requests.end();
	}

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
		PathNode* start = searchSpace->Localise(request.data.start);
		PathNode* goal = searchSpace->Localise(request.data.goal);

		// Validate request
		if (Validate(request, start, goal))
		{
			// start search
			if (pathPlanner->StartSearch(start, goal))
			{
				// terminate the request if the search is complete
				TerminateRequest(request);
			}
		}		
	}

	// Resume search
	void ResumeSearch(PathRequest& request)
	{
		if (pathPlanner->ResumeSearch())
		{
			// terminate the request if the search is complete
			TerminateRequest(request);
		}
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

	// Terminate request
	void TerminateRequest(PathRequest& request)
	{
		// get the path
		Path path;
		pathPlanner->GetPath(path);

		// terminate
		PathRequestResultStatus resultStatus = path.size() > 0 ? PathRequestResultStatus::PathFound : PathRequestResultStatus::PathNotFound;
		TerminateRequest(request, resultStatus, path);
	}

	void TerminateRequest(PathRequest& request, PathRequestResultStatus resultStatus, Path& path)
	{
		// mark as completed
		request.state = PathRequest::State::COMPLETED;

		// notify the result
		request.data.onPathRequestResult(request.id, resultStatus, path);

		// cancel current request
		CancelRequest(request.id);
	}
};

#endif // !PATHFINDER_H

