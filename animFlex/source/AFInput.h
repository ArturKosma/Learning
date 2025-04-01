#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>

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

struct FAFTouch
{
	FAFTouch()
		: origin(0, 0), lastFrame(0, 0), newLocation(0, 0)
	{
		
	}

	FAFTouch(glm::ivec2 new_origin)
		: origin(new_origin)
	{
		lastFrame = origin;
		newLocation = lastFrame;
	}

	FAFTouch(const FAFTouch& other)
		: origin(other.origin)
	{
		lastFrame = origin;
		newLocation = lastFrame;
	}

	FAFTouch(FAFTouch&& other) noexcept
		: origin(other.origin)
	{
		lastFrame = origin;
		newLocation = lastFrame;
	}

	FAFTouch& operator=(const FAFTouch& other)
	{
		origin = other.origin;
		lastFrame = origin;
		newLocation = lastFrame;

		return *this;
	}

	glm::ivec2 origin = glm::ivec2(0, 0);
	glm::ivec2 lastFrame = glm::ivec2(0, 0);
	glm::ivec2 newLocation = glm::ivec2(0, 0);
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

	void RegisterTouch(int id, const glm::ivec2& location);
	void UnregisterTouch(int id);

	void UpdateCursorPosState();
	void UpdateStrokeState();

	void Input_FreeViewMode_Pressed();
	void Input_FreeViewMode_Released();

	struct GLFWwindow* m_window = nullptr;

	std::unordered_map<std::string, FAFBoundAction> m_boundActionMappings = {};
	std::unordered_map<std::string, FAFBoundAxis> m_boundAxisMappings = {};
	std::unordered_map<int, float> m_keystate = {};
	std::unordered_map<int, FAFTouch> m_touchstate = {};

	double m_cursorOldXPos = 0.0;
	double m_cursorOldYPos = 0.0;
	double m_cursorNewXPos = 0.0;
	double m_cursorNewYPos = 0.0;


	bool m_freeView = false;
};
