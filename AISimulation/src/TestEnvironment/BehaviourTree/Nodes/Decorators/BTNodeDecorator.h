#ifndef BT_NODE_DECORATOR_H
#define BT_NODE_DECORATOR_H

#include "../BTNode.h"

class BTNodeDecorator : public BTNode
{
protected:

	// child
	std::unique_ptr<BTNode> child;

public:

	// Set child
	void SetChild(std::unique_ptr<BTNode>&& child_)
	{
		child = std::move(child_);
	}

	virtual void OnAbort(BTBlackboard& blackboard) final
	{
		// signal abort down the tree
		child->Abort(blackboard);
	}
};

#endif // !BT_NODE_DECORATOR_H
