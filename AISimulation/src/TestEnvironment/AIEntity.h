#ifndef AI_ENTITY_H
#define AI_ENTITY_H

#include "GameObject.h"
#include "BehaviourTree/BehaviourTree.h"

struct WorldState
{
	bool isTimeToSleep{ false };
	bool isDoorOpen{ false };
};

class AIEntity : public GameObject
{
	// BehaviourTree
	BehaviourTree behaviourTree;

	// blackboard
	BTBlackboard blackboard;

	// path
	int pathIndex{ -1 };
	std::vector<MathGeom::Vector3> path;

public:

	// Constructor
	AIEntity()
	{
		behaviourTree.Build();

		blackboard.Set("isWorking", false);
		blackboard.Set("isSleeping", false);
	}

	// Update
	void Update(const WorldState& worldState)
	{
		blackboard.Set("isTimeToSleep", worldState.isTimeToSleep);
		blackboard.Set("isTimeToWork", !worldState.isTimeToSleep);
		blackboard.Set("isDoorOpen", worldState.isDoorOpen);

		behaviourTree.Update(blackboard);

		FollowPath();
	}

	// Set path
	void SetPath(const std::vector<MathGeom::Vector3>& newPath)
	{
		path = newPath;
		pathIndex = path.size() > 0 ? 0 : -1;
	}


private:

	// Follow path
	void FollowPath()
	{
		if (pathIndex == -1)
		{
			// no path
			return;
		}

		// make sure the object is not stationary
		SetStationary(false);

		auto& targetPos = path[pathIndex];
		auto& currentPos = transform.position;
		
		// check if we reach the target
		const float DISTANCE_TO_TARGET = 5.0f;
		if (MathGeom::Distance(currentPos, targetPos) < DISTANCE_TO_TARGET)
		{
			pathIndex++;

			if (pathIndex >= (int)path.size())
			{
				// end of the path reached
				pathIndex = -1;
				SetStationary(true);
				return;
			}
			else
			{
				targetPos = path[pathIndex];
			}
		}

		// calculate and add seek steering force
		MathGeom::Vector3 desiredVelocity = MathGeom::Normalise(targetPos - currentPos) * 12.0f;
		MathGeom::Vector3 steering = desiredVelocity - Velocity();
		AddForce(steering);
	}
	
};

#endif // !AI_ENTITY_H

