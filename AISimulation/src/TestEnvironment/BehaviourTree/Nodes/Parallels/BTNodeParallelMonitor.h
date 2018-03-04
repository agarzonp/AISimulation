#ifndef BT_NODE_PARALLEL_MONITOR_H
#define	BT_NODE_PARALLEL_MONITOR_H

#include "BTNodeParallel.h"

#include "../Conditions/BTNodeCondition.h"

class BTNodeParallelMonitor : public BTNodeParallel
{
public:

	BTNodeParallelMonitor() = delete;
	BTNodeParallelMonitor(Policy success, Policy failure)
		: BTNodeParallel(success, failure)
	{
	}

	// Add condition
	void AddCondition(std::unique_ptr<BTNodeBaseCondition>&& condition)
	{
		children.insert(children.begin(), std::move(condition));
	}

	// Add behaviour
	void AddBehaviour(std::unique_ptr<BTNode>&& behaviour)
	{
		children.push_back(std::move(behaviour));
	}

private:
	using BTNodeParallel::AddChild;

};

#endif // !BT_NODE_PARALLEL_MONITOR_H

