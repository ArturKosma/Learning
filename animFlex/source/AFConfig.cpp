#include "AFConfig.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <glad/glad.h>

#include "AFUtility.h"

AFConfig& AFConfig::GetInstance()
{
	static AFConfig config;
	return config;
}

bool AFConfig::Init(const std::string& filepath)
{
	m_configFilepath = filepath;

	const bool parsed = ParseConfig();

	return parsed;
}

const AFConfig::FAFConfigMappings& AFConfig::GetConfigMappings() const
{
	return m_configMappings;
}

AFConfig::AFConfig()
{

}

AFConfig::~AFConfig()
{

}

bool AFConfig::ParseConfig()
{
	m_configMappings.actionMappings.clear();
	m_configMappings.axisMappings.clear();

	std::ifstream file = std::ifstream(m_configFilepath);
	if(!file)
	{
		printf("%s%s.\n", "Failed to open config file: ", m_configFilepath.c_str());
		return false;
	}

	if (file.bad() || file.fail())
	{
		file.close();
		return false;
	}

	std::string line;
	while (std::getline(file, line)) 
	{
		// Remove trailing '\r'.
		if(!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		// Skip empty lines.
		if (std::all_of(line.begin(), line.end(), [](unsigned char c) { return std::isspace(c); })) 
		{
			continue;
		}

		// Erase all white characters from the line.
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());

		// If all that's left is nothingness.
		if(line.size() == 0)
		{
			continue;
		}

		// Opening square bracket is a new config class.
		if(line[0] == '[')
		{
			m_currentParseMode = EAFParseMode::None;
		}

		// Found input actions.
		if(line == "[Input.Actions]")
		{
			m_currentParseMode = EAFParseMode::InputAction;
			continue;
		}

		// Found input axis.
		if(line == "[Input.Axes]")
		{
			m_currentParseMode = EAFParseMode::InputAxis;
			continue;
		}

		switch (m_currentParseMode)
		{
		case EAFParseMode::None:
		{
			break;
		}
		case EAFParseMode::InputAction:
		{
			ParseInputAction(line);
			break;
		}
		case EAFParseMode::InputAxis:
		{
			ParseInputAxis(line);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	file.close();
	return true;
}

void AFConfig::ParseInputAction(const std::string& line)
{
	// Find the equal sign.
	const size_t pos = line.find('=');

	// Something wrong with the line.
	if (pos == std::string::npos) 
	{
		return;
	}

	// Split the string into two parts.
	const std::string& beforeEqual = line.substr(0, pos);
	const std::string& afterEqual = line.substr(pos + 1);

	// Map the string to actual input int.
	const int intKey = AFUtility::GetInputKeyFromString(afterEqual);
	if(intKey == -1)
	{
		return;
	}

	m_configMappings.actionMappings.insert_or_assign(beforeEqual, intKey);
}

void AFConfig::ParseInputAxis(const std::string& line)
{
	// Find the equal sign.
	const size_t pos = line.find('=');

	// Something wrong with the line.
	if (pos == std::string::npos)
	{
		return;
	}

	// Split the string into two parts.
	const std::string& beforeEqual = line.substr(0, pos);
	const std::string& afterEqual = line.substr(pos + 1);

	// Find the ';' sign which splits the axis into positive and negative.
	size_t posAxis = afterEqual.find(';');

	// No split for some reason.
	if (posAxis == std::string::npos)
	{
		return;
	}

	// Get the two sides of axis.
	const std::string& afterEqualLeft = afterEqual.substr(0, posAxis);
	const std::string& afterEqualRight = afterEqual.substr(posAxis + 1);

	// Extract input key strings and axis value strings.
	auto extract = [](const std::string& axisLine) -> std::pair<std::string, std::string>
		{
			const size_t posValue = axisLine.find('(');
			const std::string& keyString = axisLine.substr(0, posValue);
			const std::string& valueString = axisLine.substr(posValue + 1, axisLine.find(')') - posValue - 1); // Extract number from between parentheses.

			return { keyString, valueString };
		};

	// Extract strings.
	const std::pair<std::string, std::string>& afterEqualLeftPair = extract(afterEqualLeft);
	const std::pair<std::string, std::string>& afterEqualRightPair = extract(afterEqualRight);

	// Find int keys.
	const int intKeyLeft = AFUtility::GetInputKeyFromString(afterEqualLeftPair.first);
	if (intKeyLeft == -1)
	{
		return;
	}

	const int intKeyRight = AFUtility::GetInputKeyFromString(afterEqualRightPair.first);
	if (intKeyRight == -1)
	{
		return;
	}

	// Get axis float values.
	const float leftFloat = static_cast<float>(std::stoi(afterEqualLeftPair.second));
	const float rightFloat = static_cast<float>(std::stoi(afterEqualRightPair.second));

	FAFAxis axis;
	axis.keyToValue1 = { intKeyLeft, leftFloat };
	axis.keyToValue2 = { intKeyRight, rightFloat };

	m_configMappings.axisMappings.insert_or_assign(beforeEqual, axis);
}
