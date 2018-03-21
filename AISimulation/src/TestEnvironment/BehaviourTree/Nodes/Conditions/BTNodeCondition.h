#ifndef BT_NODE_CONDITION_H
#define BT_NODE_CONDITION_H

#include <string>

#include "../BTNode.h"

class BTNodeBaseCondition : public BTNode
{
	public:
		virtual bool Check(BTBlackboard& blackboard) = 0;
};

template <class Condition>
class BTNodeCondition : public BTNodeBaseCondition
{
public:

	bool Check(BTBlackboard& blackboard) final
	{
		Condition& condition = static_cast<Condition&> (*this);
		return condition.IsSatisfied(blackboard);
	}

protected:

	State OnRun(BTBlackboard& blackboard) final
	{
		if (Check(blackboard))
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

	bool IsSatisfied(const BTBlackboard& blackboard)
	{
		// check whatever is needed
		return true;
	}
};

*/

#endif // !BT_NODE_CONDITION_H

