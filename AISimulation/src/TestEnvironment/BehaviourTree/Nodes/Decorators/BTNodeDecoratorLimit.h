#ifndef BT_NODE_DECORATOR_LIMIT_H
#define BT_NODE_DECORATOR_LIMIT_H

#include "BTNodeDecorator.h"

class BTNodeDecoratorLimit : public BTNodeDecorator
{
	int limit{ 0 };
	int count{ 0 };

public:

	BTNodeDecoratorLimit(int limit_)
		: BTNodeDecorator()
		, limit(limit_)
	{
	}

	State OnRun(BTBlackboard& blackBoard) final
	{
		if (count > limit)
		{
			return State::FAILED;
		}

		count++;

		return child->Run(blackBoard);
	}
};

#endif // !BT_NODE_DECORATOR_UNTIL_FAIL_H

