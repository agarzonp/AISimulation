#ifndef BT_NODE_CONDITION_H
#define BT_NODE_CONDITION_H

#include <string>

#include "../BTNode.h"

template <class Condition>
class BTNodeCondition : public BTNode
{
protected:

	State OnRun(BTBlackboard& blackboard) final
	{
		Condition& condition = static_cast<Condition&> (*this);
		if (condition.IsSatisfied(blackboard))
		{
			return State::SUCCEEDED;
		}

		return State::FAILED;
	}
};

/*

class MyCustomConditionExample : public BTNodeCondition<MyCustomConditionExample>
{
public:

	bool IsSatisfied(const BTBlackboard& blackBoard)
	{
		// check whatever is needed
		return true;
	}
};

*/

#endif // !BT_NODE_CONDITION_H

