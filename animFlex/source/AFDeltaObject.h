#pragma once
#include <string>
#include <unordered_map>

class AFDeltaObject
{
public:

	static AFDeltaObject& Get();

	void ResetValue(const std::string& name);
	float SetValue(const std::string& name, float value);
	float GetDelta(const std::string& name) const;

private:

	std::unordered_map<std::string, float> m_values;
	std::unordered_map<std::string, float> m_deltas;
	AFDeltaObject();
};
