#ifndef SEARCH_SPACE_H
#define SEARCH_SPACE_H

#include <vector>

#include "../../Render/RenderUtils.h"

#include "../PathNode.h"

// Search space type
enum class SearchSpaceType
{
	OCTILE_GRID
};

// Search space data
struct SearchSpaceData
{
	SearchSpaceType searchSpaceType;
	MathGeom::Vector3 anchorPosition;
	MathGeom::Vector3 worldSize;
};

// Seaerch space
class SearchSpace
{
protected:

	// nodes
	std::vector<PathNode> nodes;

	// search space data
	SearchSpaceData searchSpaceData;

	// node renderable
	Renderable nodeRenderable;

public:

	// Constructors
	SearchSpace() = default;
	SearchSpace(const SearchSpaceData& data) 
		: searchSpaceData(data) 
	{ 
		Validate(); 
	}

	// Build
	virtual void Build() = 0;

	// Localise
	virtual PathNode* Localise(const MathGeom::Vector3& position) = 0;

	// Debug render
	void DebugRender(const MathGeom::Matrix4& viewProjection)
	{
		DebugRenderNodes(viewProjection);
	}

private:

	// Validate
	void Validate()
	{
		assert(searchSpaceData.worldSize.x > 0);
		assert(searchSpaceData.worldSize.y > 0);
		assert(searchSpaceData.worldSize.z > 0);
	}

	// Debug render nodes
	void DebugRenderNodes(const MathGeom::Matrix4& viewProjection)
	{
		for (auto& node : nodes)
		{
			Transform transform;
			transform.position = node.position;

			RenderUtils::RenderCube(viewProjection, transform, static_cast<unsigned>(node.type));
		}
	}
};

#endif // !SEARCH_SPACE_H

