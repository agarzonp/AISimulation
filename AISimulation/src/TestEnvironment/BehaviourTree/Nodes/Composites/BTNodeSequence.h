#ifndef BT_NODE_SEQUENCE_H
#define	BT_NODE_SEQUENCE_H

#include "BTNodeComposite.h"

class BTNodeSequence : public BTNodeComposite
{
public:

	BTNodeSequence() = default;

protected:

	bool LoopBreakConditionSatisfied() final
	{
		return (state != State::SUCCEEDED);
	}

};

#endif // !BT_NODE_SEQUENCE_H

