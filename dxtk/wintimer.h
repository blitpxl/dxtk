#pragma once
#include "control.h"

class WinTimer : public Control
{
	UINT32 interval;
	static inline UINT32 id = 0;
public:
	WinTimer(Control* parent, UINT32 interval);
	void start();
	void stop();
	void reset();
	void invoke();
};