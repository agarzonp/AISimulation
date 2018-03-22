#ifndef NAVGRID_H
#define NAVGRID_H

#include "SearchSpace.h"

class NavGrid : public SearchSpace
{
	// cell size
	float cellSize { 1.0f };

	// total cells
	size_t totalCellsX { 0 };
	size_t totalCellsY { 0 };
	size_t totalCellsZ { 0 };

public:
	
	// Constructors
	NavGrid() = default;
	NavGrid(const SearchSpaceData& data, float gridCellSize)
		: SearchSpace(data)
		, cellSize(gridCellSize)
	{ 
	}

	// Build
	void Build() override
	{
		// Create grid
		CreateGrid();

		// Note: To be removed when create the grid from static world
		CreateTestLayout();
	}

	// Localise
	PathNode* Localise(const MathGeom::Vector3& position) override
	{
		MathGeom::Vector3 anchorToPos = position - searchSpaceData.anchorPosition;
		if (	anchorToPos.x >= 0 && anchorToPos.x <= searchSpaceData.worldSize.x 
			&&	anchorToPos.z >= 0 && anchorToPos.z <= searchSpaceData.worldSize.z)
		{
			anchorToPos /= cellSize;
			return GetNode(int(anchorToPos.x), int(anchorToPos.z));
		}

		return nullptr;
	}

private:

	// Create Grid
	void CreateGrid()
	{
		// clear current nodes
		nodes.clear();

		// calculate total cells
		auto& anchorPos = searchSpaceData.anchorPosition;
		auto& worldSize = searchSpaceData.worldSize;

		totalCellsX = int(worldSize.x / cellSize);
		totalCellsY = 1;
		totalCellsZ = int(worldSize.z / cellSize);
		size_t totalCells = totalCellsX * totalCellsY * totalCellsZ;

		// create the nodes
		nodes.reserve(totalCells);

		float halfCellSize = cellSize * 0.5f;
		MathGeom::Vector3 nodePos;
		for (size_t cellZ = 0; cellZ < totalCellsZ; cellZ++)
		{
			nodePos.z = anchorPos.z + cellZ * cellSize + halfCellSize;

			for (size_t cellX = 0; cellX < totalCellsX; cellX++)
			{
				nodePos.x = anchorPos.x + cellX * cellSize + halfCellSize;
				nodes.emplace_back(PathNode(nodePos));
			}
		}

		// Compute adyacency
		for (size_t i = 0; i < nodes.size(); i++)
		{
			int cellX = i % totalCellsX;
			int cellZ = i / totalCellsX;

			SetAdjacency(nodes[i], GetNode(cellX + 1, cellZ - 1), PathNodeAdjacency::TOP_RIGHT);
			SetAdjacency(nodes[i], GetNode(cellX + 1, cellZ), PathNodeAdjacency::RIGHT);
			SetAdjacency(nodes[i], GetNode(cellX + 1, cellZ + 1), PathNodeAdjacency::BOTTOM_RIGHT);
			SetAdjacency(nodes[i], GetNode(cellX , cellZ + 1), PathNodeAdjacency::BOTTOM);
		}
	}

	// Get node
	PathNode* GetNode(int x, int z)
	{
		if (x >= 0 && x < int(totalCellsX) && z >= 0 && z < int(totalCellsZ))
		{
			size_t nodeIndex = z * totalCellsX + x;
			return &nodes[nodeIndex];
		}

		return nullptr;
	}

	// Set Adjacency
	void SetAdjacency(PathNode& nodeA, PathNode* nodeB, PathNodeAdjacency adjacency)
	{
		switch (adjacency)
		{
		case PathNodeAdjacency::TOP_RIGHT: 
			nodeA.neighbours[(int)PathNodeAdjacency::TOP_RIGHT] = nodeB;
			if (nodeB)
			{
				nodeB->neighbours[(int)PathNodeAdjacency::BOTTOM_LEFT] = &nodeA;
			}
			break;
		case PathNodeAdjacency::RIGHT:
			nodeA.neighbours[(int)PathNodeAdjacency::RIGHT] = nodeB;
			if (nodeB)
			{
				nodeB->neighbours[(int)PathNodeAdjacency::LEFT] = &nodeA;
			}
			break;
		case PathNodeAdjacency::BOTTOM_RIGHT:
			nodeA.neighbours[(int)PathNodeAdjacency::BOTTOM_RIGHT] = nodeB;
			if (nodeB)
			{
				nodeB->neighbours[(int)PathNodeAdjacency::TOP_LEFT] = &nodeA;
			}
			break;
		case PathNodeAdjacency::BOTTOM:
			nodeA.neighbours[(int)PathNodeAdjacency::BOTTOM] = nodeB;
			if (nodeB)
			{
				nodeB->neighbours[(int)PathNodeAdjacency::TOP] = &nodeA;
			}
			break;
		default:
			assert(false);
			break;
		}
	}

	// Create test layout
	void CreateTestLayout()
	{
		for (size_t x = 1; x < totalCellsX; x += 3)
		{
			for (size_t z = 1; z < totalCellsZ; z +=  3)
			{
				// 2x2 block
				size_t startX = x;
				size_t startZ = z;
				for (size_t blockX = startX; blockX < startX + 2; blockX++)
				{
					for (size_t blockZ = startZ; blockZ < startZ + 2; blockZ++)
					{
						PathNode* node = GetNode(blockX, blockZ);
						if (!node)
						{
							// early exit
							break;
						}
						
						node->type = PathNodeType::BLOCKED;
					}
				}
			}
		}

		std::srand((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
		int count = 3;
		while (count > 0)
		{
			int n = std::rand() % nodes.size();
			if (nodes[n].type == PathNodeType::UNBLOCKED)
			{
				nodes[n].type = PathNodeType::BLOCKED;
				count--;
			}
		}
	}
};

#endif // !NAVGRID_H
