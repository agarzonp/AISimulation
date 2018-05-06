#ifndef PATH_REQUEST_SCHEDULER_H
#define PATH_REQUEST_SCHEDULER_H

#include <map>

#include "PathRequest.h"
#include "PathRequestQueue.h"

#include "../SearchSpace/SearchSpace.h"
#include "../PathPlanner/PathPlanner.h"

class PathRequestScheduler
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

	// Set search space
	void SetSearchSpace(std::shared_ptr<SearchSpace> searchSpace)
	{
		this->searchSpace = searchSpace;
	}

	// Set path planner
	void SetPathPlanner(std::shared_ptr<PathPlanner> pathPlanner)
	{
		this->pathPlanner = pathPlanner;
	}

	// Add request
	PathRequestId AddRequest(const PathRequestData& requestData)
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

		return request.id;
	}

	// Cancel request
	void CancelRequest(PathRequestId requestId)
	{
		requestQueue.Remove(requestId);
		requests.erase(requestId);
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

#endif // !PATH_REQUEST_SCHEDULER_H

