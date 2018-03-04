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

	// Set
	void Set(std::vector<std::unique_ptr<BTNodeBaseCondition>>& conditions, std::vector<std::unique_ptr<BTNode>>& behaviours)
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

	// Add
	void Add(std::unique_ptr<BTNodeBaseCondition>& condition)
	{
		children.insert(children.begin(), std::move(condition));
	}

	void Add(std::unique_ptr<BTNode>& behaviour)
	{
		children.push_back(std::move(behaviour));
	}

private:
	using BTNodeParallel::AddChild;

};

#endif // !BT_NODE_PARALLEL_MONITOR_H

