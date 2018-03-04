#ifndef BT_NODE_BLACKBOARD_CONDITION_H
#define BT_NODE_BLACKBOARD_CONDITION_H

#include <string>

#include "BTNodeCondition.h"

class BTNodeBlackboardCondition : public BTNodeCondition<BTNodeBlackboardCondition>
{
	std::string key;
	BTBlackboardOperator btOperator;
	BTBlackboardValue value;

public:

	template<typename T>
	BTNodeBlackboardCondition(const std::string& key_, BTBlackboardOperator btOperator_, const T& value_)
		: BTNodeCondition<BTNodeBlackboardCondition>()
		, key(key_)
		, btOperator(btOperator_)
	{
		value.Set(value_);
	}

	bool IsSatisfied(const BTBlackboard& blackboard)
	{
		return blackboard.IsSatisfied(key, btOperator, value);
	}
};
#endif // !BT_NODE_BLACKBOARD_CONDITION_H

