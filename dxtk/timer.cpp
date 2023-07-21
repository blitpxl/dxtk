#include "timer.h"

Timer::Timer(float interval)
: running(false), is_reset(false)
{
	this->interval = (int)interval*1000;
}

void Timer::timerThread()
{
	while (running)
	{
		std::this_thread::sleep_for (std::chrono::microseconds(interval));
		if (running && !is_reset)
			invokeSignal("timeout");
		is_reset = false;
	}
}

void Timer::start()
{
	running = true;
	std::thread(&Timer::timerThread, this).detach();
}

void Timer::stop()
{
	running = false;
}

void Timer::reset()
{
	is_reset = true;
}