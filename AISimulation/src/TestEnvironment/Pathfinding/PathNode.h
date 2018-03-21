#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <array>

// PathNodeType
enum class PathNodeType
{
	BLOCKED,
	UNBLOCKED
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
	PathNode(const MathGeom::Vector3& pos) : position(pos) {}

	// position
	MathGeom::Vector3 position;

	// adjacent neighbours of current PathNode
	static const int MAX_NEIGHBOURS = 8;
	std::array<PathNode*, (int)PathNodeAdjacency::MAX_ADJACENCY > neighbours;

	// type
	PathNodeType type;

	// costs
	float hCost{ 0.0f };
	float gCost{ 0.0f };
	float fCost{ 0.0f };

	// parent node
	std::shared_ptr<PathNode> parent;
};


#endif // !PATH_NODE_H

