#ifndef BT_NODE_ACTION_WORK_H
#define BT_NODE_ACTION_WORK_H

#include "BTNodeAction.h"

class BTNodeActionWork : public BTNodeAction<BTNodeActionWork>
{
public:

	// Start
	void Start(BTBlackboard& blackBoard)
	{
		BT_NODE_DEBUG_PRINT("Working...");
	}

	// End
	void End(BTBlackboard& blackBoard)
	{
	}

	// Execute
	State Execute(const BTBlackboard& blackBoard)
	{
		// Note: Not the correct way to finish an action, this is just for testing purpose
		if (blackBoard.IsSatisfied("isTimeToWork", BTBlackboardOperator::IS_EQUAL, BTBlackboardValue(false)))
		{
			return State::FAILED;
		}

		return State::SUCCEEDED;
	}
};
#endif // !BT_NODE_ACTION_WORK_H

