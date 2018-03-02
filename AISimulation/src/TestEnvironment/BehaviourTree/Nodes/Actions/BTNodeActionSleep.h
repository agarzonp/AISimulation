#ifndef BT_NODE_ACTION_SLEEP
#define BT_NODE_ACTION_SLEEP

#include "BTNodeAction.h"

class BTNodeActionSleep : public BTNodeAction<BTNodeActionSleep>
{
public:

	// Start
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Sleeping...");
	}

	// End
	void End(BTBlackboard& blackboard)
	{
	}

	// Execute
	State Execute(BTBlackboard& blackboard)
	{
		blackboard.Set("isWorking", false);
		blackboard.Set("isSleeping", true);
		return State::SUCCEEDED;
	}
};
#endif // !BT_NODE_ACTION_SLEEP

