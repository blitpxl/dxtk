#include "dxobject.h"

DxObject::~DxObject()
{
	invokeSignal("delete");
}

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
	auto x = registry[signalName];
	for (auto it = x.begin(); it != x.end(); it++)
	{
		for (std::function<void(void)> callback : it->second)
		{
			callback();
		}
	}
}