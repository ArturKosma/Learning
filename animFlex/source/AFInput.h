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

	static void BindAction(const std::string& actionName, const std::function<void()>& actionFunction, EAFKeyAction action);
	static void BindAxis(const std::string& axis, const std::function<void(float)>& fun);

private:

	static void Init();

	static AFInput& GetInstance();

	AFInput();
	~AFInput();

	void Tick();

	void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosCallback(struct GLFWwindow* window, double posX, double posY);
	void OnScrollCallback(struct GLFWwindow* window, double xscroll, double yscroll);

	std::unordered_map<std::string, FAFBoundAction> m_boundActionMappings = {};
	std::unordered_map<std::string, FAFBoundAxis> m_boundAxisMappings = {};
	std::unordered_map<int, float> m_keystate = {};

	double m_cursorXPos = 0.0f;
	double m_cursorYPos = 0.0f;

	bool m_mouseLock = false;
};
