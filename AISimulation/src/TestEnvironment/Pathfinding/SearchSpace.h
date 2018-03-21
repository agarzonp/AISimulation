#ifndef SEARCH_SPACE_H
#define SEARCH_SPACE_H

#include "PathNode.h"

class SearchSpace
{
public:

	// SearchSpaceData
	struct SearchSpaceData
	{
		MathGeom::Vector3 anchorPosition;
		MathGeom::Vector3 worldSize;
	};

	SearchSpace(const SearchSpaceData& data) : searchSpaceData(data) {}

	// Build
	virtual void Build() = 0;

	// Localise
	virtual PathNode* Localise(const MathGeom::Vector3& position) = 0;

private:

	// Validate
	bool Validate()
	{
		assert(searchSpaceData.worldSize.x > 0);
		assert(searchSpaceData.worldSize.y > 0);
		assert(searchSpaceData.worldSize.z > 0);
	}

protected:

	// nodes
	std::vector<PathNode> nodes;

	// search space data
	SearchSpaceData searchSpaceData;
};

#endif // !SEARCH_SPACE_H

