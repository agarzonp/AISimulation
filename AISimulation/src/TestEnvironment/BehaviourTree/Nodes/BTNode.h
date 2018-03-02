#ifndef BT_NODE_H
#define BT_NODE_H

#include "../../../Utils/Timer.h"
#include "../Blackboard/BTBlackboard.h"

#define ALLOW_DEBUG 1
#if ALLOW_DEBUG 
#define BT_NODE_DEBUG_PRINT(X) printf("%s\n", X)
#else
#define BT_NODE_DEBUG_PRINT(X) ;
#endif

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

	State Run(BTBlackboard& blackboard)
	{
		if (state != State::RUNNING)
		{
			OnEnter(blackboard);
		}

		state = OnRun(blackboard);

		if (state != State::RUNNING)
		{
			OnExit(blackboard);
		}

		return state;
	}

protected:

	virtual void OnEnter(BTBlackboard& blackboard) {};
	virtual void OnExit(BTBlackboard& blackboard) {};
	virtual State OnRun(BTBlackboard& blackboard) = 0;
	
};

#endif // !BT_NODE_H

