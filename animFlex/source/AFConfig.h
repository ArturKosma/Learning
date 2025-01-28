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
	std::pair<int, float> keyToValue1 = {};
	std::pair<int, float> keyToValue2 = {};
};

class AFConfig
{
	using ActionKeyMap = std::unordered_map<std::string, int>;
	using AxisKeyMap = std::unordered_map<std::string, FAFAxis>;

public:

	static AFConfig& GetInstance();

	struct FAFConfigMappings
	{
		ActionKeyMap actionMappings = ActionKeyMap();
		AxisKeyMap axisMappings = AxisKeyMap();
	};

	bool Init(const std::string& filepath);

	const FAFConfigMappings& GetConfigMappings() const;

private:

	AFConfig();
	~AFConfig();

	bool ParseConfig();

	void ParseInputAction(const std::string& line);
	void ParseInputAxis(const std::string& line);

	EAFParseMode m_currentParseMode = EAFParseMode::None;

	std::string m_configFilepath = std::string();
	FAFConfigMappings m_configMappings = FAFConfigMappings();
};
