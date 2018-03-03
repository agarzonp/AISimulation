#ifndef BT_NODE_PARALLEL_MONITOR_H
#define	BT_NODE_PARALLEL_MONITOR_H

#include "BTNodeParallel.h"

class BTNodeParallelMonitor : public BTNodeParallel
{
public:

	BTNodeParallelMonitor() = delete;
	BTNodeParallelMonitor(Policy success, Policy failure)
		: BTNodeParallel(success, failure)
	{
	}

	// TO-DO: make sure we are passing conditions (We need unified BTNodeCondition and BTNodeBlackboardCondition)
	void Set(std::vector<std::unique_ptr<BTNode>>& conditions, std::vector<std::unique_ptr<BTNode>>& behaviours)
	{
		for (auto& condition : conditions)
		{
			children.emplace_back(std::move(condition));
		}
		
		for (auto& behaviour : behaviours)
		{
			children.emplace_back(std::move(behaviour));
		}
	}

private:
	using BTNodeParallel::AddChild;

};

#endif // !BT_NODE_PARALLEL_MONITOR_H

