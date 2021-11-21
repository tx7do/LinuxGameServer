#pragma once

#include <condition_variable>
#include <mutex>
#include <atomic>


class Event
{
public:
	enum EventType
	{
		EVENT_MANUALRESET,
		EVENT_AUTORESET
	};

	explicit Event(EventType type = EVENT_AUTORESET);

	~Event();

	void set();

	void wait();

	void wait(long milliseconds);

	bool tryWait(long milliseconds);

	void reset();

private:
	Event(const Event&);
	Event& operator = (const Event&);

private:
	std::atomic_bool  _auto;
	std::atomic_bool  _state;
	std::mutex  _mutex;
	std::condition_variable  _cond;
};

