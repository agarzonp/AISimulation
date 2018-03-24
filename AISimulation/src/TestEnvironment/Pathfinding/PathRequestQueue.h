#ifndef PATH_REQUEST_QUEUE_H
#define PATH_REQUEST_QUEUE_H

#include <deque>

#include "PathRequest.h"

class PathRequestQueue
{
	// queue
	std::deque<PathRequest*> queue;

public:

	// Enqueue
	bool Enqueue(PathRequest& request)
	{
		switch (request.data.priority)
		{
		case PathRequestPriority::NORMAL:
		{
			queue.push_back(&request);
			return true;
		}
		default:
			assert(false);
			break;
		}

		return false;
	}

	// Remove
	void Remove(PathRequestId requestId)
	{
		for (auto requestIt = queue.begin(); requestIt != queue.end(); ++requestIt)
		{
			if ((*requestIt)->id == requestId)
			{
				queue.erase(requestIt);
				return;
			}
		}
	}

	// Get request
	PathRequest* GetRequest()
	{
		if (queue.size() > 0)
		{
			return queue.front();
		}

		return nullptr;
	}
};


#endif // !PATH_REQUEST_QUEUE_H

