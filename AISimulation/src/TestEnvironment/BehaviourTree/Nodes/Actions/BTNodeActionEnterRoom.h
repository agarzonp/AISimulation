#ifndef BT_NODE_ACTION_ENTER_ROOM_H
#define BT_NODE_ACTION_ENTER_ROOM_H

#include "BTNodeAction.h"

class BTNodeActionEnterRoom : public BTNodeAction<BTNodeActionEnterRoom>
{
	Timer timer;

public:
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Entering into the room...");

		blackboard.Set("enterRoom", true);

		timer.Start(2000);
	}

	void End(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Inside room!");

		timer.Stop();

		blackboard.Set("enterRoom", false);
	}

	void Cancel(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Entering room cancelled!");
	}

	State Execute(BTBlackboard& blackboard)
	{
		if (timer.HasTicked())
		{
			return state = State::SUCCEEDED;
		}

		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_ENTER_ROOM_H
