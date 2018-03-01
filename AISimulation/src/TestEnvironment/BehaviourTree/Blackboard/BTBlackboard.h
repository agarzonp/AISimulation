#ifndef BT_BLACKBOARD_H
#define BT_BLACKBOARD_H

#include "BTBlackboardValue.h"

class BTBlackboard
{
	std::map<std::string, BTBlackboardValue> blackboard;

public:

	template<class T>
	void Set(const std::string& key, const T& value)
	{
		auto it = blackboard.find(key);
		if (it == blackboard.end())
		{
			BTBlackboardValue blackboardValue;
			blackboardValue.Set(value);
			blackboard.insert({ key, blackboardValue });
		}
		else
		{
			it->second.Set(value);
		}
	}

	bool IsSatisfied(const std::string& key, BTBlackboardOperator btOperator, const BTBlackboardValue& value) const
	{
		auto it = blackboard.find(key);
		assert(it != blackboard.end());
		if (it != blackboard.end())
		{
			return it->second.isSatisfied(btOperator, value);
		}

		return false;
	}
};
#endif // !BT_BLACKBOARD_H

