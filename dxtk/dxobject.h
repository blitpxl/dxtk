#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

class DxObject
{
	std::unordered_map<std::string, std::vector<std::function<void(void)>>> signals;
public:
	DxObject() = default;
	~DxObject() { invokeSignal("destroyed"); }

	void registerSignal(std::string const& signal_name, std::function<void(void)> function);
	void unregisterSignal(std::string const& signal_name);
	void invokeSignal(std::string const& signal_name);
};