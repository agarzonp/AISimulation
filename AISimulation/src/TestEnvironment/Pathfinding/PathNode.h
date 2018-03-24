#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <array>

#include "../Render/Renderable.h"

// PathNodeType
enum class PathNodeType
{
	BLOCKED = 0x000000, // black
	UNBLOCKED = 0xFFFFFF // white
};

// PathNodeAdjacency
enum class PathNodeAdjacency
{
	TOP_LEFT,
	TOP,
	TOP_RIGHT,
	RIGHT,
	BOTTOM_RIGHT,
	BOTTOM,
	BOTTOM_LEFT,
	LEFT,

	MAX_ADJACENCY
};

// PathNode
struct PathNode
{
	// Constructors
	PathNode() = default;
	PathNode(const MathGeom::Vector3& pos) 
		: position(pos) 
	{
		Init();
	}

	// position
	MathGeom::Vector3 position;

	// adjacent neighbours of current PathNode
	std::array<PathNode*, (int)PathNodeAdjacency::MAX_ADJACENCY > neighbours;

	// type
	PathNodeType type { PathNodeType::UNBLOCKED };

	// costs
	float hCost{ 0.0f };
	float gCost{ 0.0f };
	float fCost{ 0.0f };

	// parent node
	PathNode* parent{ nullptr };

private:

	void Init()
	{
		for (int i = 0; i < (int)PathNodeAdjacency::MAX_ADJACENCY; i++)
		{
			neighbours[i] = nullptr;
		}
	}
};


#endif // !PATH_NODE_H

