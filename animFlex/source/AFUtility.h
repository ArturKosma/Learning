#pragma once

#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>

class AFUtility
{
public:

	AFUtility() = delete;
	~AFUtility() = delete;

	static int GetInputKeyFromString(const std::string& inputKeyString)
	{
		std::unordered_map<std::string, int> keymap =
		{
			{"KEY_W", GLFW_KEY_W},
			{"KEY_A", GLFW_KEY_A},
			{"KEY_S", GLFW_KEY_S},
			{"KEY_D", GLFW_KEY_D},
			{"KEY_Q", GLFW_KEY_Q},
			{"KEY_E", GLFW_KEY_E},
			{"MOUSE_BUTTON_RIGHT", GLFW_MOUSE_BUTTON_RIGHT}
		};

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
