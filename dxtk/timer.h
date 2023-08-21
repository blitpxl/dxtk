#pragma once
#include "dxobject.h"
#include <thread>
#include <chrono>

class Timer : public DxObject
{
	bool is_reset;
	void timerThread();
public:
	int interval;
	bool running;
	bool run;

	Timer(float interval);
	void start();
	void stop();
	void reset();
};