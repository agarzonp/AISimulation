#ifndef BT_NODE_COMPOSITE
#define BT_NODE_COMPOSITE

#include <functional>
#include <vector>
#include <memory>

#include "../BTNode.h"

class BTNodeComposite : public BTNode
{
protected:

	// children
	std::vector<std::unique_ptr<BTNode>> children;

	// running child 
	int runningChildIndex { -1 };

public:

	// Add child
	void AddChild(std::unique_ptr<BTNode>&& child)
	{
		children.emplace_back(std::move(child));
	}
	
protected:

	State OnRun(BTBlackboard& blackBoard) final
	{
		if (state != State::RUNNING)
		{
			// loop until a child didn´t break
			for (size_t childIndex = 0; childIndex < children.size(); childIndex++)
			{
				state = children[childIndex]->Run(blackBoard);

				if (OnRunBreak())
				{
					runningChildIndex = childIndex;
					break;
				}
			}
		}
		else
		{
			state = children[runningChildIndex]->Run(blackBoard);
		}

		if (state != State::RUNNING)
		{
			// no running child
			runningChildIndex = -1;
		}

		return state;
	}

	virtual bool OnRunBreak() = 0;

};

#endif // !BT_NODE_COMPOSITE

