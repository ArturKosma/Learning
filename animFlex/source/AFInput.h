#pragma once
#include <functional>
#include <string>
#include <unordered_map>

enum class EAFKeyAction
{
	Pressed,
	Released
};

struct FAFBoundAction
{
	std::vector<std::function<void()>> onPressedFunctors;
	std::vector<std::function<void()>> onReleasedFunctors;
};

struct FAFBoundAxis
{
	std::vector<std::function<void(float)>> axisFunctors;
	std::pair<int, float> keyToValue1;
	std::pair<int, float> keyToValue2;
};

class AFInput
{
	friend class AFApp;
	friend class AFWindow;

public:

	static AFInput& GetInstance();

	void BindAction(const std::string& actionName, const std::function<void()>& actionFunction, EAFKeyAction action);
	void BindAxis(const std::string& axis, const std::function<void(float)>& fun);

	bool GetFreeViewMode() const;

private:

	static void Init(struct GLFWwindow* window);

	AFInput();
	~AFInput();

	void Tick();

	void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosCallback(struct GLFWwindow* window, double posX, double posY);
	void OnScrollCallback(struct GLFWwindow* window, double xscroll, double yscroll);

	void Input_FreeViewMode_Pressed();
	void Input_FreeViewMode_Released();

	struct GLFWwindow* m_window = nullptr;

	std::unordered_map<std::string, FAFBoundAction> m_boundActionMappings = {};
	std::unordered_map<std::string, FAFBoundAxis> m_boundAxisMappings = {};
	std::unordered_map<int, float> m_keystate = {};

	int m_cursorXPos = 0;
	int m_cursorYPos = 0;

	bool m_freeView = false;
};
