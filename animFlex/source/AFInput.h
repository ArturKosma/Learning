#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

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

	static void BindAction(const std::string& actionName, const std::function<void()>& actionFunction, EAFKeyAction action);
	static void BindAxis(const std::string& axis, const std::function<void(float)>& fun);

	static bool GetFreeViewMode();

private:

	static void Init(struct GLFWwindow* window);

	AFInput();
	~AFInput();

	void Tick();

	void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosCallback(struct GLFWwindow* window, double posX, double posY);
	void OnScrollCallback(struct GLFWwindow* window, double xscroll, double yscroll);

#ifdef __EMSCRIPTEN__
	void OnTouchStart(int eventType, const EmscriptenTouchEvent* e);
	void OnTouchMove(int eventType, const EmscriptenTouchEvent* e);
	void OnTouchEnd(int eventType, const EmscriptenTouchEvent* e);
#endif

	void UpdateCursorPosState();

	void Input_FreeViewMode_Pressed();
	void Input_FreeViewMode_Released();

	struct GLFWwindow* m_window = nullptr;

	std::unordered_map<std::string, FAFBoundAction> m_boundActionMappings = {};
	std::unordered_map<std::string, FAFBoundAxis> m_boundAxisMappings = {};
	std::unordered_map<int, float> m_keystate = {};

	double m_cursorOldXPos = 0.0;
	double m_cursorOldYPos = 0.0;
	double m_cursorNewXPos = 0.0;
	double m_cursorNewYPos = 0.0;


	bool m_freeView = false;
};
