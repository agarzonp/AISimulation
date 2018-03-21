#ifndef BT_NODE_ACTION_WORK_H
#define BT_NODE_ACTION_WORK_H

#include "BTNodeAction.h"

class BTNodeActionWork : public BTNodeAction<BTNodeActionWork>
{
public:

	// Start
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Working...");
	}

	// End
	void End(BTBlackboard& blackboard)
	{
	}

	// Cancel
	void Cancel(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Work cancelled!");

		blackboard.Set("isWorking", false);
	}

	// Execute
	State Execute(BTBlackboard& blackboard)
	{
		blackboard.Set("isWorking", true);
		blackboard.Set("isSleeping", false);
		return State::SUCCEEDED;
	}
};
#endif // !BT_NODE_ACTION_WORK_H

