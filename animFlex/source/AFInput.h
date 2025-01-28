#pragma once
#include <functional>
#include <string>
#include <unordered_map>

enum class EAFInputMode
{
	Default,
	FreeView
};

enum class EAFKeyAction
{
	Pressed,
	Released
};

struct FAFBoundAction
{
	std::function<void()> onPressed;
	std::function<void()> onReleased;
};

struct FAFBoundAxis
{
	std::function<void(float)> fun;
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

private:

	void Init();

	AFInput();
	~AFInput();

	void Tick();

	void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosCallback(struct GLFWwindow* window, double xoffset, double yoffset);
	void OnScrollCallback(struct GLFWwindow* window, double xscroll, double yscroll);

	EAFInputMode m_inputMode = EAFInputMode::Default;
	std::unordered_map<std::string, FAFBoundAction> m_boundActionMappings = {};
	std::unordered_map<std::string, FAFBoundAxis> m_boundAxisMappings = {};
	std::unordered_map<int, bool> m_keystate = {};

	double m_cursorXPos = 0.0f;
	double m_cursorYPos = 0.0f;

	bool m_mouseLock = false;
};
