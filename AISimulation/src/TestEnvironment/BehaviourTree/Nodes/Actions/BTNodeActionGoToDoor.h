#ifndef BT_NODE_ACTION_GOTO_DOOR_H
#define BT_NODE_ACTION_GOTO_DOOR_H

#include "BTNodeAction.h"

class BTNodeActionGoToDoor: public BTNodeAction<BTNodeActionGoToDoor>
{
	Timer timer;

public:
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Going to door...");

		blackboard.Set("gotoDoor", true);

		timer.Start(6000);
	}

	void End(BTBlackboard& blackboard)
	{
		timer.Stop();

		blackboard.Set("gotoDoor", false);
	}

	void Cancel(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Going to door cancelled!");
	}

	State Execute(BTBlackboard& blackboard)
	{
		if (timer.HasTicked())
		{	
			BT_NODE_DEBUG_PRINT("In front of door!");
			return state = State::SUCCEEDED;
		}
	
		return state = State::RUNNING;
	}

};

#endif // !BT_NODE_ACTION_GOTO_DOOR_H

