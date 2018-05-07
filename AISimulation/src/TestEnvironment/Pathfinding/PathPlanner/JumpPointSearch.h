#ifndef JUMP_POINT_SEARCH_H
#define JUMP_POINT_SEARCH_H

#include "OpenClosePathPlanner.h"

class JumpPointSearch : public OpenClosePathPlanner
{
  // Jump direction is a direct map to PathNodeAdjacency
  using JumpDirection = PathNodeAdjacency;

public:

	// Constructor
	JumpPointSearch(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace) 
		: OpenClosePathPlanner(data, searchSpace)
	{
	}

private:

  // Expand
  void Expand(PathNode* current) final
  {
    std::vector<PathNode*> successors;

    // Identify successors
    IdentifySuccessors(current, successors);

    // update cost for each successor
	for (auto successor : successors)
	{
		UpdateCost(current, successor);
	}

    // push current in close
    close.push_back(current);
  }

  // Identify successors
  void IdentifySuccessors(PathNode* current, std::vector<PathNode*>& successors)
  {
    // clear current successors
    successors.clear();

    // prune neighbours
    std::vector<PathNode*> neighbours;
    Prune(current, neighbours);

    // jump
    for (auto neighbour : neighbours)
    {
      if (auto jumpPoint = Jump(current, GetJumpDirection(current, neighbour)))
      {
        successors.push_back(jumpPoint);
      }
    }
  }

  // Prune
  void Prune(PathNode* current, std::vector<PathNode*>& neighbours)
  {
    if (current->parent)
    {
		bool flagForcedNodes = false;
		Prune(current, neighbours, GetJumpDirection(current->parent, current), flagForcedNodes);
    }
	else
	{
		// current node is the start node, so consider all walkable neighbours
		for (auto neighbour : current->neighbours)
		{
			if (searchSpace->IsValidAdjacency(current, neighbour))
			{
				neighbours.push_back(neighbour);
			}
		}
	}
  }

  // Prune
  void Prune(PathNode* current, std::vector<PathNode*>& neighbours, JumpDirection direction, bool flagForcedNodes)
  {
	  switch (direction)
	  {
	  case JumpDirection::RIGHT:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::RIGHT, JumpDirection::TOP_RIGHT, JumpDirection::BOTTOM_RIGHT, JumpDirection::TOP, JumpDirection::BOTTOM, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::LEFT:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::LEFT, JumpDirection::TOP_LEFT, JumpDirection::BOTTOM_LEFT, JumpDirection::TOP, JumpDirection::BOTTOM, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::TOP:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::TOP, JumpDirection::TOP_RIGHT, JumpDirection::TOP_LEFT, JumpDirection::RIGHT, JumpDirection::LEFT, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::BOTTOM:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::RIGHT, JumpDirection::LEFT, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::TOP_RIGHT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::TOP, JumpDirection::RIGHT, JumpDirection::TOP_RIGHT, JumpDirection::TOP_LEFT, JumpDirection::LEFT, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::TOP_LEFT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::TOP, JumpDirection::LEFT, JumpDirection::TOP_LEFT, JumpDirection::TOP_RIGHT, JumpDirection::RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::BOTTOM, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::BOTTOM_RIGHT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::RIGHT, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::LEFT, JumpDirection::TOP_RIGHT, JumpDirection::TOP, flagForcedNodes);
		  break;
	  }
	  case JumpDirection::BOTTOM_LEFT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::LEFT, JumpDirection::BOTTOM_LEFT, JumpDirection::BOTTOM_RIGHT, JumpDirection::RIGHT, JumpDirection::TOP_LEFT, JumpDirection::TOP, flagForcedNodes);
		  break;
	  }
	  default:
		  assert(false);
		  break;
	  }
  }

  // Prune straight move
  void PruneStraightMove(
	  PathNode* current, 
	  std::vector<PathNode*>& neighbours, 
	  JumpDirection right, 
	  JumpDirection topRight, 
	  JumpDirection bottomRight, 
	  JumpDirection top, 
	  JumpDirection bottom,
	  bool flagForcedNodes)
  {
	  PathNode* rightNode = current->GetNeighbour((int)right);
	  if (IsWalkable(rightNode))
	  {
		  neighbours.push_back(rightNode);

		  PathNode* topRightNode = current->GetNeighbour((int)topRight);
		  PathNode* bottomRightNode = current->GetNeighbour((int)bottomRight);

		  PathNode* topNode = current->GetNeighbour((int)top);
		  PathNode* bottomNode = current->GetNeighbour((int)bottom);

		  if (IsWalkable(topRightNode) && !IsWalkable(topNode))
		  {
			  topRightNode->isForced = flagForcedNodes;
			  neighbours.push_back(topRightNode); 
		  }

		  if (IsWalkable(bottomRightNode) && !IsWalkable(bottomNode))
		  {
			  bottomRightNode->isForced = flagForcedNodes;
			  neighbours.push_back(bottomRightNode);
		  }
	  }
  }

  // Prune diagonal move
  void PruneDiagonalMove(
	  PathNode* current, 
	  std::vector<PathNode*>& neighbours,
	  JumpDirection top,
	  JumpDirection right, 
	  JumpDirection topRight, 
	  JumpDirection topLeft,
	  JumpDirection left,
	  JumpDirection bottomRight,
	  JumpDirection bottom,
	  bool flagForcedNodes)
  {
	  PathNode* topNode = current->GetNeighbour(int(top));
	  PathNode* rightNode = current->GetNeighbour(int(right));
	  PathNode* topRightNode = current->GetNeighbour(int(topRight));

	  if (IsWalkable(topNode))
	  {
		  neighbours.push_back(topNode);

		  PathNode* topLeftNode = current->GetNeighbour(int(topLeft));
		  PathNode* leftNode = current->GetNeighbour(int(left));

		  if (IsWalkable(topLeftNode) && !IsWalkable(leftNode))
		  {
			  topLeftNode->isForced = flagForcedNodes;
			  neighbours.push_back(topLeftNode); 
		  }
	  }

	  if (IsWalkable(rightNode))
	  {
		  neighbours.push_back(rightNode);

		  PathNode* bottomRightNode = current->GetNeighbour(int(bottomRight));
		  PathNode* bottomNode = current->GetNeighbour(int(bottom));

		  if (IsWalkable(bottomRightNode) && !IsWalkable(bottomNode))
		  {
			  bottomRightNode->isForced = flagForcedNodes;
			  neighbours.push_back(bottomRightNode);
		  }
	  }

	  if (IsWalkable(topRightNode) && (IsWalkable(topNode) || IsWalkable(rightNode)))
	  {
		  neighbours.push_back(topRightNode);
	  }
  }

  // Get jump direction
  JumpDirection GetJumpDirection(PathNode* fromNode, PathNode* toNode)
  {
	  // work out jump direction from normalised move
	  // Straight moves: [1,0] [-1,0] [0,1] [0,-1]
	  // Diagonal moves: [1,1] [1,-1] [-1,1] [-1,-1]

	  MathGeom::Vector2 dir = GetNormalisedMove(fromNode, toNode);
	  if (dir.x == 0 || dir.y == 0)
	  {
		  // straight moves
		  if (dir.x == 1)
		  {
			  return JumpDirection::RIGHT;
		  }
		  else if (dir.x == -1)
		  {
			  return JumpDirection::LEFT;
		  }
		  else if (dir.y == 1)
		  {
			  return JumpDirection::TOP;
		  }
		  else
		  {
			  return JumpDirection::BOTTOM;
		  }
	  }
	  else
	  {
		  // diagonal moves
		  if (dir.x > 0)
		  {
			  return dir.y > 0 ? JumpDirection::TOP_RIGHT : JumpDirection::BOTTOM_RIGHT;
		  }
		  else
		  {
			  return dir.y > 0 ? JumpDirection::TOP_LEFT : JumpDirection::BOTTOM_LEFT;
		  }
	  }

	  return JumpDirection::MAX_ADJACENCY;
  }

  // Get normalised move
  MathGeom::Vector2 GetNormalisedMove(PathNode* fromNode, PathNode* toNode)
  {
	  MathGeom::Vector2 dir;
	  if (fromNode->position.x < toNode->position.x)
	  {
		  dir.x = 1;
	  }
	  else if (fromNode->position.x > toNode->position.x)
	  {
		  dir.x = -1;
	  }

	  if (fromNode->position.z < toNode->position.z)
	  {
		  dir.y = -1;
	  }
	  else if (fromNode->position.z > toNode->position.z)
	  {
		  dir.y = 1;
	  }

	  return dir;
  }

  // Is walkable
  bool IsWalkable(PathNode* node) { return node && node->type != PathNodeType::BLOCKED; }

  // Jump
  PathNode* Jump(PathNode* current, JumpDirection direction)
  {
    PathNode* jumpPoint = current->neighbours[(int)direction];
   
    if (!IsWalkable(jumpPoint))
    {
      // No way to jump outside the grid or to a blocked node
      return nullptr;
    }

    if (jumpPoint == goal)
    {
      // jumpPoint is the goal node!
      return jumpPoint;
    }

    // Check if there exists a neighbour of jumpPoint that is forced
	if (HasForcedNeighbours(jumpPoint, direction))
	{
		return jumpPoint;
	}
  
    // check diagonal jump
    if (IsDiagonalDirection(direction))
    {
      // find straight jump point
      std::array<JumpDirection, 2> straightDirections = GetStraightJumpDirections(direction);
      if (Jump(jumpPoint, straightDirections[0]) || Jump(jumpPoint, straightDirections[1]))
      {
        return jumpPoint;
      }
    }

    // keep jumping
    return Jump(jumpPoint, direction);
  }

  // Has forced neighbours
  bool HasForcedNeighbours(PathNode* current, JumpDirection direction)
  {
	  bool hasForcedNeighbours = false;
	  bool flagForcedNodes = true;

	  // prune neighbours
	  std::vector<PathNode*> neighbours;
	  Prune(current, neighbours, direction, flagForcedNodes);
	  for (auto n : neighbours)
	  {
		  if (n->isForced)
		  {
			  hasForcedNeighbours = true;

			  // make sure we clear is forced flag
			  for (auto nf : neighbours)
			  {
				  nf->isForced = false;
			  }
			  break;
		  }
	  }

	  return hasForcedNeighbours;
  }

  // Is Diagonal Direction
  bool IsDiagonalDirection(JumpDirection direction)
  {
    return (direction == JumpDirection::TOP_LEFT || direction == JumpDirection::BOTTOM_LEFT || direction == JumpDirection::TOP_RIGHT || direction == JumpDirection::BOTTOM_RIGHT);
  }

  // Get StraighJumpDirections
  std::array<JumpDirection, 2> GetStraightJumpDirections(JumpDirection jumpDirection)
  {
    std::array<JumpDirection, 2> directions;

    switch (jumpDirection)
    {
    case JumpDirection::TOP:
      directions[0] = JumpDirection::TOP;
      directions[1] = JumpDirection::TOP;
      break;
    case JumpDirection::BOTTOM:
      directions[0] = JumpDirection::BOTTOM;
      directions[1] = JumpDirection::BOTTOM;
      break;
    case JumpDirection::RIGHT:
      directions[0] = JumpDirection::RIGHT;
      directions[1] = JumpDirection::RIGHT;
      break;
    case JumpDirection::LEFT:
      directions[0] = JumpDirection::LEFT;
      directions[1] = JumpDirection::LEFT;
      break;
    case JumpDirection::TOP_LEFT:
      directions[0] = JumpDirection::TOP;
      directions[1] = JumpDirection::LEFT;
      break;
    case JumpDirection::TOP_RIGHT:
      directions[0] = JumpDirection::TOP;
      directions[1] = JumpDirection::RIGHT;
      break;
    case JumpDirection::BOTTOM_LEFT:
      directions[0] = JumpDirection::BOTTOM;
      directions[1] = JumpDirection::LEFT;
      break;
    case JumpDirection::BOTTOM_RIGHT:
      directions[0] = JumpDirection::BOTTOM;
      directions[1] = JumpDirection::RIGHT;
      break;
    }

    return directions;
  }
};

#endif // !JUMP_POINT_SEARCH_H

