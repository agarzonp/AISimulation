#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <map>

#include "PathRequest.h"
#include "PathRequestQueue.h"
#include "SearchSpace.h"

class Pathfinder
{
	// path requests
	std::map<PathRequestId, PathRequest> requests;

	// request queue
	PathRequestQueue requestQueue;

	// request count
	size_t requestCount{ 0 };

	// search space
	std::unique_ptr<SearchSpace> searchSpace;

public:

	// Handler for the path request
	class PathRequestHandler
	{
		// id
		PathRequestId id { 0 };
		Pathfinder* pathfinder { nullptr };

		public:

			// Constructors
			PathRequestHandler() = default;
			PathRequestHandler(PathRequestId id_, Pathfinder* pathfinder_) : id(id_), pathfinder(pathfinder_) {}

			// Desctructor
			~PathRequestHandler() { Cancel(); }

			// operator bool
			operator bool() const { return id > 0 && pathfinder; }

			// Cancel
			void Cancel()
			{
				if (pathfinder)
				{
					pathfinder->CancelRequest(id);
					pathfinder = nullptr;
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
		FindPaths();

		// TO-DO remove any invalid or completed requests
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
			// Start search
			break;
		case PathRequest::State::INTERRUPTED:
			// Resume search
			break;
		case PathRequest::State::COMPLETED:
		case PathRequest::State::INVALID:
		case PathRequest::State::RUNNING:
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
		// TO-DO
	}

	// Resume searh
	void ResumeSearch(PathRequest& request)
	{
		// TO-DO
	}
};

#endif // !PATHFINDER_H

