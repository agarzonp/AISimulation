#ifndef BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H
#define BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H

#include "BTNodeAction.h"

class BTNodeActionRequestOpenDoor : public BTNodeAction<BTNodeActionRequestOpenDoor>
{
	static const int MAX_COUNT = 10;
	int count{ MAX_COUNT };
public:
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Requesting to open the door...");

		blackboard.Set("requestOpenDoor", true);
		count = MAX_COUNT;
	}

	void End(BTBlackboard& blackboard)
	{
		blackboard.Set("requestOpenDoor", false);
		count = MAX_COUNT;
	}

	State Execute(BTBlackboard& blackboard)
	{
		count--;
		if (count == 0)
		{
			BT_NODE_DEBUG_PRINT("Door opened!");

			blackboard.Set("isDoorOpen", true);

			return state = State::SUCCEEDED;
		}

		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_REQUEST_OPEN_DOOR_ROOM_H
