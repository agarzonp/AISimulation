#ifndef BT_NODE_BLACKBOARD_CONDITION_H
#define BT_NODE_BLACKBOARD_CONDITION_H

#include <string>

#include "../BTNode.h"

class BTNodeBlackboardCondition : public BTNode
{
	std::string key;
	BTBlackboardOperator btOperator;
	BTBlackboardValue value;

public:

	template<typename T>
	BTNodeBlackboardCondition(const std::string& key_, BTBlackboardOperator btOperator_, const T& value_)
		:BTNode()
		, key(key_)
		, btOperator(btOperator_)
	{
		value.Set(value_);
	}

	State OnRun(BTBlackboard& blackBoard) final
	{
		if (blackBoard.IsSatisfied(key, btOperator, value))
		{
			return State::SUCCEEDED;
		}

		return State::FAILED;
	}
};
#endif // !BT_NODE_BLACKBOARD_CONDITION_H

