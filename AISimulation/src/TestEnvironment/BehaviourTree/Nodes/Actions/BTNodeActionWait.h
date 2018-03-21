#ifndef BT_NODE_ACTION_WAIT_H
#define	BT_NODE_ACTION_WAIT_H

#include "BTNodeAction.h"

class BTNodeActionWait : public BTNodeAction<BTNodeActionWait>
{
	Timer timer;

public:

	// Constructors
	BTNodeActionWait() = default;
	BTNodeActionWait(Timer::Milliseconds time)
		: BTNodeAction<BTNodeActionWait>()
		, timer(time)
	{
	}

	// Start
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Waiting...");
		timer.Start();
	}

	// End
	void End(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Wait ended!");
		timer.Stop();
	}

	// Cancel
	void Cancel(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Wait cancelled!");
	}

	// Execute
	State Execute(BTBlackboard& blackboard)
	{
		if (timer.HasTicked())
		{	
			return State::SUCCEEDED;
		}

		return State::RUNNING;
	}
};

#endif // !BT_NODE_ACTION_WAIT_H

