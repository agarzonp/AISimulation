#ifndef BT_NODE_PARALLEL
#define BT_NODE_PARALLEL

#include <functional>
#include <vector>
#include <memory>

#include "../BTNode.h"

class BTNodeParallel : public BTNode
{
public:

	enum class Policy
	{
		RequireOne,
		RequireAll
	};

protected:

	// children
	std::vector<std::unique_ptr<BTNode>> children;

	// policies
	Policy successPolicy;
	Policy failurePolicy;

public:

	BTNodeParallel() = delete;
	BTNodeParallel(Policy success, Policy failure)
		: BTNode()
		, successPolicy(success)
		, failurePolicy(failure)
	{
	}

	// Add child
	void AddChild(std::unique_ptr<BTNode>& child)
	{
		children.emplace_back(std::move(child));
	}

protected:

	void OnExit(BTBlackboard& blackboard) final
	{
		// make sure any running child is aborted
		for (auto& child : children)
		{
			child->Abort(blackboard);
		}
	}

	void OnAbort(BTBlackboard& blackboard) final
	{
		for (auto& child : children)
		{
			child->Abort(blackboard);
		}
	}

	State OnRun(BTBlackboard& blackboard) final
	{
		int successCount = 0;
		int failureCount = 0;

		for (auto& child : children)
		{
			switch (child->Run(blackboard))
			{
			case BTNode::State::INVALID:
			case BTNode::State::ABORTED:
			case BTNode::State::FAILED:
				failureCount++;
				break;
			case BTNode::State::SUCCEEDED:
				successCount++;
				break;
			case BTNode::State::RUNNING:
				break;
			}

			if (failurePolicy == Policy::RequireOne && failureCount == 1)
			{
				return State::FAILED;
			}

			if (successPolicy == Policy::RequireOne && successCount == 1)
			{
				return State::SUCCEEDED;
			}
		}

		if (failurePolicy == Policy::RequireAll && failureCount == children.size())
		{
			return State::FAILED;
		}
		
		if (successPolicy == Policy::RequireAll && successCount == children.size())
		{
			return State::SUCCEEDED;
		}

		return State::RUNNING;
	}
};

#endif // !BT_NODE_COMPOSITE
