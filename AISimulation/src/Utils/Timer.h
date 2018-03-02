#ifndef TIMER_H
#define TIMER_H

#include <cassert>
#include <chrono>

class Timer
{
public:

	using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
	using Milliseconds = std::chrono::milliseconds;

private:

	Milliseconds duration{ 0 };
	TimePoint finishTime;

	enum class State
	{
		STOPPED,
		RUNNING
	};

	State state{ State::STOPPED };

public:

	Timer() = default;
	Timer(Milliseconds ms) : duration(ms) {}
	Timer(size_t ms) : duration(Milliseconds(ms)) {}

	template<class Duration>
	void Start(Duration duration_)
	{
		duration = Milliseconds(duration_);
		Start();
	}

	void Start()
	{
		assert(state != State::RUNNING);

		state = State::RUNNING;
		finishTime = std::chrono::system_clock::now() + duration;
	}

	void Stop()
	{
		state = State::STOPPED;
	}

	bool HasTicked()
	{
		return state == State::RUNNING ? std::chrono::system_clock::now() >= finishTime : false;
	}

};

#endif // !TIMER_H

