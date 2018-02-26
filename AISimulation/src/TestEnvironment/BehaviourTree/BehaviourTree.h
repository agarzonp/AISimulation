#ifndef BEHAVIOUR_TREE_H
#define BEHAVIOUR_TREE_H

#include <memory>

#include "Blackboard/BTBlackboard.h"
#include "Nodes/BTNodes.h"

class BehaviourTree
{
	// blackboard
	BTBlackboard blackBoard;

	// root
	std::unique_ptr<BTNode> root;

public:

	void Update()
	{
		root->Run(blackBoard);
	}
};

#endif // !BEHAVIOUR_TREE_H

