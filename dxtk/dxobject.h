#pragma once
#include <string_view>
#include <unordered_map>
#include <vector>
#include <functional>

class DxObject
{
	std::unordered_map<std::string_view, std::vector<std::function<void(void)>>> signals;
public:
	DxObject() = default;
	~DxObject() { invokeSignal("destroyed"); }

	void registerSignal(std::string_view signal_name, std::function<void(void)> function);
	void unregisterSignal(std::string_view signal_name);
	void invokeSignal(std::string_view signal_name);
};