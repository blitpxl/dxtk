#pragma once
#include "dxobject.h"
#include <thread>
#include <chrono>

class Timer : public DxObject
{
	int interval;
	bool running;
	void timerThread();
public:
	Timer(float interval);
	void start();
	void stop();
};