#include "timer.h"

Timer::Timer(int interval)
: running(false)
{
	this->interval = interval;
}

void Timer::timerThread()
{
	while (running)
	{
		std::this_thread::sleep_for (std::chrono::milliseconds(interval));
		invokeSignal("timeout");
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