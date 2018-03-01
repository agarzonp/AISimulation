#ifndef BT_NODE_ACTION_GOTO_DOOR_H
#define BT_NODE_ACTION_GOTO_DOOR_H

#include "BTNodeAction.h"

class BTNodeActionGoToDoor: public BTNodeAction<BTNodeActionGoToDoor>
{
	static const int MAX_DISTANCE = 5;
	int distance{ MAX_DISTANCE };

public:
	void Start(BTBlackboard& blackBoard)
	{
		blackBoard.Set("gotoDoor", true);
		distance = MAX_DISTANCE;
	}

	void End(BTBlackboard& blackBoard)
	{
		blackBoard.Set("gotoDoor", false);
		distance = MAX_DISTANCE;
	}

	State Execute(BTBlackboard& blackBoard)
	{
		BT_NODE_DEBUG_PRINT("Going to door...");

		distance--;
		if (distance == 0)
		{
			BT_NODE_DEBUG_PRINT("In front of door!");
			return state = State::SUCCEEDED;
		}
	
		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_GOTO_DOOR_H

