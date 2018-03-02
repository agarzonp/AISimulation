#ifndef BT_NODE_ACTION_H
#define BT_NODE_ACTION_H

#include "../BTNode.h"

template <class Action>
class BTNodeAction : public BTNode
{
public:

	void OnEnter(BTBlackboard& blackboard) final
	{
		state = State::RUNNING;

		Action& action = static_cast<Action&> (*this);
		return action.Start(blackboard);
	}

	void OnExit(BTBlackboard& blackboard) final
	{
		Action& action = static_cast<Action&> (*this);
		return action.End(blackboard);
	}

	State OnRun(BTBlackboard& blackboard) final
	{
		Action& action = static_cast<Action&> (*this);
		return action.Execute(blackboard);
	}
};

/*

class MyCustomActionExample : public BTNodeAction<MyCustomActionExample>
{
public:

	void Start(BTBlackboard& blackBoard)
	{
		// do init stuff
	}

	void End(BTBlackboard& blackBoard)
	{
		// do deinit stuff
	}

	State Execute(BTBlackboard& blackBoard)
	{
		// execute the action
		// set the correct state
		// return the state
		return state;
	}
};

*/

#endif // !BT_NODE_ACTION_H

