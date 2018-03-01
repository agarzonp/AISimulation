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
	void AddChild(std::unique_ptr<BTNode>& child)
	{
		children.emplace_back(std::move(child));
	}
	
protected:

	void OnEnter(BTBlackboard& blackBoard) final
	{
		runningChildIndex = -1;
	}

	void OnExit(BTBlackboard& blackBoard) final
	{
		runningChildIndex = -1;
	}

	State OnRun(BTBlackboard& blackBoard) final
	{
		if (state != State::RUNNING)
		{
			LoopStartingFromChild(0, blackBoard);
		}
		else
		{
			state = children[runningChildIndex]->Run(blackBoard);

			// continue checking the rest of children
			if (state != State::RUNNING && !LoopBreakConditionSatisfied())
			{
				LoopStartingFromChild(runningChildIndex + 1, blackBoard);
			}
		}

		return state;
	}

	// Loop 
	void LoopStartingFromChild(size_t startChild, BTBlackboard& blackBoard)
	{
		// loop until a child didn�t break
		for (size_t childIndex = startChild; childIndex < children.size(); childIndex++)
		{
			state = children[childIndex]->Run(blackBoard);

			if (LoopBreakConditionSatisfied())
			{
				runningChildIndex = childIndex;
				break;
			}
		}
	}

	virtual bool LoopBreakConditionSatisfied() = 0;

};

#endif // !BT_NODE_COMPOSITE

