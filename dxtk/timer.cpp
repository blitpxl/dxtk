#include "timer.h"

Timer::Timer(float interval)
: running(false)
{
	this->interval = (int)interval*1000;
}

void Timer::timerThread()
{
	while (running)
	{
		std::this_thread::sleep_for (std::chrono::microseconds(interval));
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