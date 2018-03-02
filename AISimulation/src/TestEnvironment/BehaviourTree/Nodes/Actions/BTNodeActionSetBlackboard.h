#ifndef BT_NODE_ACTION_SET_BLACKBOARD_H
#define	BT_NODE_ACTION_SET_BLACKBOARD_H

#include "BTNodeAction.h"

class BTNodeActionSetBlackboard : public BTNodeAction<BTNodeActionSetBlackboard>
{
	std::string key;
	BTBlackboardValue value;

public:

	BTNodeActionSetBlackboard() = default;

	template <typename T>
	BTNodeActionSetBlackboard(std::string key_, const T& value_)
		: BTNodeAction<BTNodeActionSetBlackboard>()
		, key(key_)
	{
		value.Set(value_);
	}

	// Start
	void Start(BTBlackboard& blackboard)
	{
	}

	// End
	void End(BTBlackboard& blackboard)
	{
	}

	// Cancel
	void Cancel(BTBlackboard& blackboard)
	{
	}

	// Execute
	State Execute(BTBlackboard& blackboard)
	{
		blackboard.Set(key, value);
		return State::SUCCEEDED;
	}
};

#endif // !BT_NODE_ACTION_SET_BLACKBOARD_H

