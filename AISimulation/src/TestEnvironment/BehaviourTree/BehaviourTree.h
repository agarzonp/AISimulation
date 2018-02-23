#ifndef BEHAVIOUR_TREE_H
#define BEHAVIOUR_TREE_H

#include <memory>

#include "Nodes/BTNode.h"

class BehaviourTree
{
	// blackboard
	BlackBoard blackBoard;

	// root
	std::unique_ptr<BTNode> root;

public:

	void Update()
	{
		root->Run(blackBoard);
	}
};

#endif // !BEHAVIOUR_TREE_H

