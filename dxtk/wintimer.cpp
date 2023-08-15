#include "wintimer.h"
#include "window.h"

WinTimer::WinTimer(Control* parent, UINT32 interval)
: Control(parent)
{
	this->interval = interval;
}

void WinTimer::start()
{
	SetTimer(resource.window->getHandle(), id, interval, NULL);
	resource.window->timers[id] = this;
	id++;
}

void WinTimer::invoke()
{
	invokeSignal("timeout");
}