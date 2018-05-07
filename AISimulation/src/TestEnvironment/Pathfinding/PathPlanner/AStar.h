#ifndef A_STAR_H
#define A_STAR_H

#include "OpenClosePathPlanner.h"

class AStar : public OpenClosePathPlanner
{
public:

	// Constructors
	AStar(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace) 
		: OpenClosePathPlanner(data, searchSpace)
	{
	}

private:

	// Expand
	void Expand(PathNode* current) final
	{
		// Update cost for each valid neighbour
		for (auto neighbour : current->neighbours)
		{
			if (!searchSpace->IsValidAdjacency(current, neighbour))
				continue;

			UpdateCost(current, neighbour);
		}

		// push current in close
		close.push_back(current);
	}
};

#endif // !A_STAR_H

