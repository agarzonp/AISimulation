#ifndef JUMP_POINT_SEARCH_H
#define JUMP_POINT_SEARCH_H

#include "PathPlanner.h"

class JumpPointSearch : public PathPlanner
{
  // open and closed list
  std::list<PathNode*> open;
  std::list<PathNode*> close;

  // start/goal nodes
  PathNode* start{ nullptr };
  PathNode* goal{ nullptr };

  // Jump direction is a direct map to PathNodeAdjacency
  using JumpDirection = PathNodeAdjacency;

public:

	// Constructor
	JumpPointSearch(const PathPlannerData& data, std::shared_ptr<SearchSpace> searchSpace) 
		: PathPlanner(data, searchSpace) 
	{
	}

  // Start search
  bool StartSearch(PathNode* start, PathNode* goal) final
  {
    // reset
    Reset();

    // set start/goal
    this->start = start;
    this->goal = goal;

    // init open list
    open.push_back(start);

    // search
    return Search();
  }

  // Resume search
  bool ResumeSearch() final
  {
    // keep searching
    return Search();
  }

  // Get path
  void GetPath(Path& path) final
  {
    if (pathFound)
    {
      // go backwards to get the path
      auto node = close.back();
      while (node)
      {
        path.push_back(node->position);
        node = node->parent;
      }

      // reverse the path
      std::reverse(path.begin(), path.end());
    }
  }

  // Debug render
  void DebugRender(const MathGeom::Matrix4& viewProjection) final
  {
    if (start && goal)
    {
		Transform transform;
		transform.position = start->position;
		RenderUtils::RenderCube(viewProjection, transform, 0x00FF00);

		transform.position = goal->position;
		RenderUtils::RenderCube(viewProjection, transform, 0x00FF00);

		Path path;
		GetPath(path);

		for (auto& p : path)
		{
			Transform transform;
			transform.position = p;
			RenderUtils::RenderCube(viewProjection, transform, 0xFF0000);
		}
    }
  }

  // Reset
  void Reset() override
  {
	  searchCompleted = false;
	  pathFound = false;

	  // clear lists
	  auto ClearList = [this](std::list<PathNode*>& list)
	  {
		  // reset nodes 
		  for (auto& node : list)
		  {
			  node->fCost = 0.0f;
			  node->gCost = 0.0f;
			  node->hCost = 0.0f;
			  node->parent = nullptr;
		  }

		  list.clear();
	  };

	  ClearList(open);
	  ClearList(close);

	  start = nullptr;
	  goal = nullptr;
  }

private:

  // Search
  bool Search()
  {
    int revolution = 0;
    while (open.size() > 0)
    {
      // get cheapest from open
      auto current = PopCheapestFromOpen();
      if (current == goal)
      {
        // path found!
        pathFound = true;
        close.push_back(current);
        break;
      }

      // Evaluate
      Evaluate(current);

      // check search allowance
      if (revolution++ >= maxRevolutions)
      {
        // allowance surpassed so exit and continue next frame
        break;
      }
    }

    searchCompleted = (pathFound || open.size() == 0);

    return searchCompleted;
  }

  // Pop cheapest from open
  PathNode* PopCheapestFromOpen()
  {
    PathNode* cheapest = open.front();
    auto cheapestIterator = open.begin();
    for (auto nodeIterator = open.begin(); nodeIterator != open.end(); ++nodeIterator)
    {
      PathNode* node = *nodeIterator;
      if (node->fCost < cheapest->fCost)
      {
        cheapestIterator = nodeIterator;
        cheapest = *cheapestIterator;
      }
      else if (node->fCost == cheapest->fCost && node->hCost < cheapest->hCost)
      {
        cheapestIterator = nodeIterator;
        cheapest = *cheapestIterator;
      }
    }

    open.erase(cheapestIterator);

    return cheapest;
  }

  // Evaluate
  void Evaluate(PathNode* current)
  {
    std::vector<PathNode*> successors;

    // Identify successors
    IdentifySuccessors(current, successors);

    // Evaluate successors
    EvaluateSuccessors(current, successors);

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
		Prune(current, neighbours, GetJumpDirection(current->parent, current));
    }
	else
	{
		// current node is the start node, so consider all walkable neighbours
		for (auto neighbour : current->neighbours)
		{
			if (IsWalkable(neighbour))
			{
				neighbours.push_back(neighbour);
			}
		}
	}
  }

  // Prune
  void Prune(PathNode* current, std::vector<PathNode*>& neighbours, JumpDirection direction)
  {
	  switch (direction)
	  {
	  case JumpDirection::RIGHT:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::RIGHT, JumpDirection::TOP_RIGHT, JumpDirection::BOTTOM_RIGHT, JumpDirection::TOP, JumpDirection::BOTTOM);
		  break;
	  }
	  case JumpDirection::LEFT:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::LEFT, JumpDirection::TOP_LEFT, JumpDirection::BOTTOM_LEFT, JumpDirection::TOP, JumpDirection::BOTTOM);
		  break;
	  }
	  case JumpDirection::TOP:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::TOP, JumpDirection::TOP_RIGHT, JumpDirection::TOP_LEFT, JumpDirection::RIGHT, JumpDirection::LEFT);
		  break;
	  }
	  case JumpDirection::BOTTOM:
	  {
		  PruneStraightMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::RIGHT, JumpDirection::LEFT);
		  break;
	  }
	  case JumpDirection::TOP_RIGHT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::TOP, JumpDirection::RIGHT, JumpDirection::TOP_RIGHT, JumpDirection::TOP_LEFT, JumpDirection::LEFT, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM);
		  break;
	  }
	  case JumpDirection::TOP_LEFT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::TOP, JumpDirection::LEFT, JumpDirection::TOP_LEFT, JumpDirection::TOP_RIGHT, JumpDirection::RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::BOTTOM);
		  break;
	  }
	  case JumpDirection::BOTTOM_RIGHT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::RIGHT, JumpDirection::BOTTOM_RIGHT, JumpDirection::BOTTOM_LEFT, JumpDirection::LEFT, JumpDirection::TOP_RIGHT, JumpDirection::TOP);
		  break;
	  }
	  case JumpDirection::BOTTOM_LEFT:
	  {
		  PruneDiagonalMove(current, neighbours, JumpDirection::BOTTOM, JumpDirection::LEFT, JumpDirection::BOTTOM_LEFT, JumpDirection::BOTTOM_RIGHT, JumpDirection::RIGHT, JumpDirection::TOP_LEFT, JumpDirection::TOP);
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
	  JumpDirection bottom)
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
			  topRightNode->isForced = true;
			  neighbours.push_back(topRightNode); 
		  }

		  if (IsWalkable(bottomRightNode) && !IsWalkable(bottomNode))
		  {
			  bottomRightNode->isForced = true;
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
	  JumpDirection bottom)
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
			  neighbours.push_back(topLeftNode); // forced
		  }
	  }

	  if (IsWalkable(rightNode))
	  {
		  neighbours.push_back(rightNode);

		  PathNode* bottomRightNode = current->GetNeighbour(int(bottomRight));
		  PathNode* bottomNode = current->GetNeighbour(int(bottom));

		  if (IsWalkable(bottomRightNode) && !IsWalkable(bottomNode))
		  {
			  neighbours.push_back(bottomRightNode); // forced
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

	  // prune neighbours
	  std::vector<PathNode*> neighbours;
	  Prune(current, neighbours, direction);
	  for (auto n : neighbours)
	  {
		  if (n->isForced)
		  {
			  hasForcedNeighbours = true;
			  break;
		  }
	  }

	  // make sure we clear is forced flag
	  for (auto n : neighbours)
	  {
		  n->isForced = false;
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

  // Evaluate Successors
  void EvaluateSuccessors(PathNode* current, std::vector<PathNode*>& successors)
  {
    for (auto successor : successors)
    {
      if (std::find(close.begin(), close.end(), successor) != close.end())
      {
        // ignore if it is in close
        continue;
      }

      if (std::find(open.begin(), open.end(), successor) != open.end())
      {
        // check if this path is better
        float gCost = current->gCost + DistanceSq(current, successor);
        if (gCost < successor->gCost)
        {
          // update successor
          successor->parent = current;
          successor->gCost = gCost;
          successor->fCost = successor->hCost + gCost;
        }
      }
      else
      {
        // update successor
        successor->parent = current;
        successor->hCost = DistanceSq(successor, goal);
        successor->gCost = current->gCost + DistanceSq(current, successor);
        successor->fCost = successor->hCost + successor->gCost;

        // push in open
        open.push_back(successor);
      }
    }
  }

  float DistanceSq(PathNode* nodeA, PathNode* nodeB)
  {
    return MathGeom::DistanceSq(nodeA->position, nodeB->position);
  }
};

#endif // !JUMP_POINT_SEARCH_H

