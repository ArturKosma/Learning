#pragma once

#include <string>
#include <unordered_map>

enum class EAFParseMode
{
	None,
	InputAction,
	InputAxis
};

struct FAFAxis
{
	int key1;
	int key2;
	float axis1;
	float axis2;
};

class AFConfig
{
	using ActionKeyMap = std::unordered_map<std::string, int>;
	using AxisKeyMap = std::unordered_map<std::string, FAFAxis>;

public:

	bool Init(const std::string& filepath);

private:

	bool ParseConfig();

	void ParseInputAction(const std::string& line);
	void ParseInputAxis(const std::string& line);

	EAFParseMode m_currentParseMode = EAFParseMode::None;

	std::string m_configFilepath = std::string();
	ActionKeyMap m_actionKeyMappings = ActionKeyMap();
	AxisKeyMap m_axisKeyMappings = AxisKeyMap();
};
