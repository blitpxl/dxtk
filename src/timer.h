#pragma once
#include "dxobject.h"
#include <thread>
#include <chrono>
#include "shitlog.h"

class Timer : public DxObject
{
	int interval;
	bool running;

	void invoke();
	void timerThread();
public:
	Timer(int interval);
	void start();
	void stop();
};