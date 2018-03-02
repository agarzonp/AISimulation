#ifndef BT_NODE_ACTION_WAIT_H
#define	BT_NODE_ACTION_WAIT_H

#include <chrono>

#include "BTNodeAction.h"

class BTNodeActionWait : public BTNodeAction<BTNodeActionWait>
{
	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Milliseconds = std::chrono::milliseconds;

	Milliseconds waitTime { 0 };
	TimePoint finishTime;

public:

	// Constructors
	BTNodeActionWait() = default;
	BTNodeActionWait(Milliseconds waitTime_)
		: BTNodeAction<BTNodeActionWait>()
		, waitTime(waitTime_)
	{
	}

	// Start
	void Start(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Waiting...");
		finishTime = std::chrono::system_clock::now() + waitTime;
	}

	// End
	void End(BTBlackboard& blackboard)
	{
		BT_NODE_DEBUG_PRINT("Wait finished!");
	}

	// Execute
	State Execute(BTBlackboard& blackboard)
	{
		if (std::chrono::system_clock::now() >= finishTime)
		{	
			return State::SUCCEEDED;
		}

		return State::RUNNING;
	}
};

#endif // !BT_NODE_ACTION_WAIT_H

