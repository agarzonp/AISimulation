#ifndef AI_ENTITY_H
#define AI_ENTITY_H

#include "BehaviourTree/BehaviourTree.h"

struct WorldState
{
	bool isTimeToSleep{ false };
	bool isDoorOpen{ false };
};

class AIEntity
{
	// BehaviourTree
	BehaviourTree behaviourTree;

	// blackboard
	BTBlackboard blackboard;

public:

	AIEntity()
	{
		behaviourTree.Build();

		blackboard.Set("isWorking", false);
		blackboard.Set("isSleeping", false);
	}

	void Update(const WorldState& worldState)
	{
		blackboard.Set("isTimeToSleep", worldState.isTimeToSleep);
		blackboard.Set("isTimeToWork", !worldState.isTimeToSleep);
		blackboard.Set("isDoorOpen", worldState.isDoorOpen);

		behaviourTree.Update(blackboard);
	}
	
};

#endif // !AI_ENTITY_H

