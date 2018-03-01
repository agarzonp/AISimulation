#ifndef BT_NODE_DECORATOR_FILTER_H
#define BT_NODE_DECORATOR_FILTER_H

#include "BTNodeDecorator.h"
#include "../Conditions/BTNodeBlackboardCondition.h"

class BTNodeFilter
{
	using Filter = BTNodeBlackboardCondition;

	Filter filter;

public:
	BTNodeFilter(const Filter& filter_) : filter(filter_) {}

	bool IsSatisfied(BTBlackboard& blackBoard)
	{
		return (filter.Run(blackBoard) == BTNode::State::SUCCEEDED);
	}
};

class BTNodeDecoratorFilter : public BTNodeDecorator
{
	using Filter = BTNodeFilter;
	Filter filter;

public:

	BTNodeDecoratorFilter() = delete;
	BTNodeDecoratorFilter(const Filter& filter_)
		:BTNodeDecorator()
		, filter(filter_)
	{
	}

	State OnRun(BTBlackboard& blackBoard) final
	{
		if (filter.IsSatisfied(blackBoard))
		{
			return child->Run(blackBoard);
		}

		return State::FAILED;
	}
};

#endif // !BT_NODE_DECORATOR_FILTER_H

