#pragma once

#include "AFRenderer.h"
#include "AFModel.h"

class AFWindow
{

public:

	static AFWindow& GetInstance();

	void StartLoop();
	void Tick();

	void OnWindowResize(int newWidth, int newHeight);

	bool ShouldShutdown();

	AFWindow(const AFWindow&) = delete;
	AFWindow& operator=(const AFWindow&) = delete;
	AFWindow(AFWindow&&) = delete;
	AFWindow& operator=(AFWindow&&) = delete;

	static float deltaTime;

private:

	AFWindow();
	~AFWindow();

	void Tick_Internal(float deltaTime);

	GLFWwindow* m_window = nullptr;

	AFRenderer m_renderer = AFRenderer();
	AFModel m_model = AFModel();

	float m_previousTime = 0.0f;
};
