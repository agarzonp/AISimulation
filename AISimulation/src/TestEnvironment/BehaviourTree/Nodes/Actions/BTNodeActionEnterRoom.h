#ifndef BT_NODE_ACTION_ENTER_ROOM_H
#define BT_NODE_ACTION_ENTER_ROOM_H

#include "BTNodeAction.h"

class BTNodeActionEnterRoom : public BTNodeAction<BTNodeActionEnterRoom>
{
	static const int MAX_DISTANCE = 30;
	int distance{ MAX_DISTANCE };

public:
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Entering into the room...");

		blackboard.Set("enterRoom", true);
		distance = MAX_DISTANCE;
	}

	void End(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Inside room!");

		blackboard.Set("enterRoom", false);
		distance = MAX_DISTANCE;
	}

	State Execute(BTBlackboard& blackboard)
	{
		distance--;
		if (distance == 0)
		{
			return state = State::SUCCEEDED;
		}

		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_ENTER_ROOM_H
