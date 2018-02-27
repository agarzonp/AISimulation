#ifndef BT_NODE_H
#define BT_NODE_H

#include "../Blackboard/BTBlackboard.h"

class BTNode
{
public:

	enum class State
	{
		INVALID = -1,
		SUCCEEDED,
		FAILED,
		RUNNING
	};

protected:

	State state { State::INVALID };

public:

	virtual ~BTNode() 
	{
	}

	State Run(BTBlackboard& blackBoard)
	{
		if (state != State::RUNNING)
		{
			OnEnter(blackBoard);
		}

		state = OnRun(blackBoard);

		if (state != State::RUNNING)
		{
			OnExit(blackBoard);
		}

		return state;
	}

protected:

	virtual void OnEnter(BTBlackboard& blackBoard) {};
	virtual void OnExit(BTBlackboard& blackBoard) {};
	virtual State OnRun(BTBlackboard& blackBoard) = 0;
	
};

#endif // !BT_NODE_H
