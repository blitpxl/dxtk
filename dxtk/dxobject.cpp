#include "dxobject.h"

void DxObject::registerSignal(std::string_view signal_name, std::function<void(void)> function)
{
	signals[signal_name].push_back(function);
}

void DxObject::unregisterSignal(std::string_view signal_name)
{
	signals.erase(signal_name);
}

void DxObject::invokeSignal(std::string_view signal_name)
{
	for (std::function<void(void)> signal : signals[signal_name])
	{
		signal();
	}
}