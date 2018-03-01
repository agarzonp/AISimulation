#ifndef BT_NODE_ACTION_ENTER_ROOM_H
#define BT_NODE_ACTION_ENTER_ROOM_H

#include "BTNodeAction.h"

class BTNodeActionEnterRoom : public BTNodeAction<BTNodeActionEnterRoom>
{
	static const int MAX_DISTANCE = 2;
	int distance{ MAX_DISTANCE };

public:
	void Start(BTBlackboard& blackBoard)
	{
		blackBoard.Set("enterRoom", true);
		blackBoard.Set("sleeping", false);
		distance = MAX_DISTANCE;
	}

	void End(BTBlackboard& blackBoard)
	{
		blackBoard.Set("enterRoom", false);
		blackBoard.Set("sleeping", true); // use a FSM to transition into the sleep state?
		distance = MAX_DISTANCE;
	}

	State Execute(BTBlackboard& blackBoard)
	{
		BT_NODE_DEBUG_PRINT("Entering into the room...");

		distance--;
		if (distance == 0)
		{
			BT_NODE_DEBUG_PRINT("Inside room!");
			return state = State::SUCCEEDED;
		}

		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_ENTER_ROOM_H
