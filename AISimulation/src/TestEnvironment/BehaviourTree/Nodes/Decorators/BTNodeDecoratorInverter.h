#ifndef BT_NODE_DECORATOR_INVERTER_H
#define	BT_NODE_DECORATOR_INVERTER_H

#include "BTNodeDecorator.h"

class BTNodeDecoratorInverter : public BTNodeDecorator
{

public:

	State OnRun(BTBlackboard& blackBoard) final
	{
		state = child->Run(blackBoard);

		state = state == State::SUCCEEDED ? State::FAILED 
										  : state == State::FAILED ? State::SUCCEEDED 
										  : state;
		return state;
	}
};

#endif 
