#ifndef BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H
#define BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H

#include "BTNodeAction.h"

class BTNodeActionRequestOpenDoor : public BTNodeAction<BTNodeActionRequestOpenDoor>
{
	Timer timer;

public:
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Requesting to open the door...");

		blackboard.Set("requestOpenDoor", true);

		timer.Start(3000);
	}

	void End(BTBlackboard& blackboard)
	{
		timer.Stop();

		blackboard.Set("requestOpenDoor", false);
	}

	void Cancel(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Request open door cancelled!");
	}

	State Execute(BTBlackboard& blackboard)
	{
		if (timer.HasTicked())
		{
			BT_NODE_DEBUG_PRINT("Door opened!");

			blackboard.Set("isDoorOpen", true);

			return state = State::SUCCEEDED;
		}

		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H
