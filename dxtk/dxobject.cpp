#include "dxobject.h"

void DxObject::registerSignal(DxObject* registrant, std::string_view signalName, std::function<void(void)> callback)
{
	registry[signalName][registrant].push_back(callback);
}

void DxObject::unregisterSignal(DxObject* registrant, std::string_view signalName)
{
	if (signalName != "")
	{
		registry[signalName].erase(registrant);
	}
	else
	{
		for (const auto& [signalName, _] : registry)
		{
			registry[signalName].erase(registrant);
		}
	}
}

void DxObject::invokeSignal(std::string_view signalName)
{
	for (const auto& [registrant, callbackVector] : registry[signalName])
	{
		for (std::function<void(void)> callback : callbackVector)
		{
			callback();
		}
	}
}