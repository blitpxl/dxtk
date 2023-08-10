#pragma once
#include "dxobject.h"
#include <thread>
#include <chrono>

class Timer : public DxObject
{
	int interval;
	bool is_reset;
	void timerThread();
public:
	bool running;
	bool run;

	Timer(float interval);
	void start();
	void stop();
	void reset();
};