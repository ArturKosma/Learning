#pragma once

#include <iostream>
#include <chrono>

#include <GLFW/glfw3.h>

class AFWindow
{

public:

	static AFWindow& GetInstance();
	bool ShouldShutdown();

	void Tick();

	AFWindow(const AFWindow&) = delete;
	AFWindow& operator=(const AFWindow&) = delete;
	AFWindow(AFWindow&&) = delete;
	AFWindow& operator=(AFWindow&&) = delete;

private:

	AFWindow();
	~AFWindow();

	void Tick_Internal(float InDeltaTime);

	bool m_WantsShutdown = false;
	float m_DeltaTime = -1.0f;
	std::chrono::time_point<std::chrono::steady_clock> m_LastTimestamp = {};
};