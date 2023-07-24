#pragma once
#include <vector>
#include <unordered_map>
#include <string_view>
#include <functional>

#include "logging.h"

class DxObject
{
	std::unordered_map<std::string_view, std::unordered_map<DxObject*, std::vector<std::function<void(void)>>>> registry;
public:
	DxObject() = default;

	void registerSignal(DxObject* registrant, std::string_view signalName, std::function<void(void)> callback);
	void unregisterSignal(DxObject* registrant, std::string_view signalName = "");
	void invokeSignal(std::string_view signalName);
};