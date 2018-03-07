#ifndef PATH_REQUEST_H
#define PATH_REQUEST_H

#include <vector>
#include <functional>

// PathRequestId
using PathRequestId = size_t;

// PathRequestResult
struct PathRequestResult
{
	PathRequestId id;
	std::vector<MathGeom::Vector3> path;
};

// OnPathRequestResult
using OnPathRequestResult = std::function<void(const PathRequestResult& result)>;

// PathRequestPriority
enum class PathRequestPriority
{
	NORMAL
};

// PathRequestData
struct PathRequestData
{
	MathGeom::Vector3 start;
	MathGeom::Vector3 goal;
	PathRequestPriority priority{ PathRequestPriority::NORMAL };
	OnPathRequestResult onPathRequestResult;
};

// PathRequest
struct PathRequest
{
	enum class State
	{
		INVALID,
		QUEUED,
		RUNNING,
		INTERRUPTED,
		COMPLETED
	};

	PathRequestId id;
	PathRequestData data;
	State state;
};
#endif // !PATH_REQUEST_H

