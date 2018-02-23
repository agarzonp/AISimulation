#ifndef BT_NODE_H
#define BT_NODE_H

class BlackBoard
{
	// TO-DO
};

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

	State Run(BlackBoard& blackBoard)
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

	virtual void OnEnter(BlackBoard& blackBoard) {};
	virtual void OnExit(BlackBoard& blackBoard) {};
	virtual State OnRun(BlackBoard& blackBoard) = 0;
	
};

#endif // !BT_NODE_H

