#pragma once

#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

class AFUtility
{
public:

	AFUtility() = delete;
	~AFUtility() = delete;

	static std::unordered_map<std::string, int> GetConfigurableKeys()
	{
		std::unordered_map<std::string, int> keymap =
		{
			{"KEY_W", GLFW_KEY_W},
			{"KEY_A", GLFW_KEY_A},
			{"KEY_S", GLFW_KEY_S},
			{"KEY_D", GLFW_KEY_D},
			{"KEY_Q", GLFW_KEY_Q},
			{"KEY_E", GLFW_KEY_E},
			{"KEY_ESCAPE", GLFW_KEY_ESCAPE},
			{"MOUSE_TURN_RIGHT", 1001},
			{"MOUSE_TURN_LEFT", 1002},
			{"MOUSE_TILT_UP", 1003},
			{"MOUSE_TILT_DOWN", 1004}
		};

		return keymap;
	}

	static int GetInputKeyFromString(const std::string& inputKeyString)
	{
		std::unordered_map<std::string, int> keymap = GetConfigurableKeys();

		auto it = keymap.find(inputKeyString);
		if(it != keymap.end())
		{
			return it->second;
		}
		else
		{
			return -1;
		}
	}
};
