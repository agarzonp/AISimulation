#ifndef BT_NODE_SELECTOR
#define	BT_NODE_SELECTOR

#include "BTNodeComposite.h"

class BTNodeSelector : public BTNodeComposite
{
public:

	BTNodeSelector() = default;

protected:

	bool LoopBreakConditionSatisfied() final
	{
		return (state != State::FAILED);
	}

};

#endif // !BT_NODE_SELECTOR

