#include "AFTimerManager.h"

#include <GLFW/glfw3.h>

void AFTimerManager::AFTimer::Start()
{
	if (m_running)
	{
		return;
	}

	m_running = true;
	m_startTime = std::chrono::steady_clock::now();
}

float AFTimerManager::AFTimer::Stop()
{
	if (!m_running)
	{
		return 0.0f;
	}

	const auto stopTime = std::chrono::steady_clock::now();
	float elapsed = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - m_startTime).count() / 1000.0f;

	m_running = false;

	return elapsed;
}

AFTimerManager& AFTimerManager::GetInstance()
{
	static AFTimerManager timerManager;
	return timerManager;
}

float AFTimerManager::GetDeltaTime()
{
	return GetInstance().m_deltaTime;
}

void AFTimerManager::Init()
{
	m_previousTime = static_cast<float>(glfwGetTime());;
}

void AFTimerManager::DeltaCalc()
{
	const float currentTime = static_cast<float>(glfwGetTime());
	m_deltaTime = currentTime - m_previousTime;
	m_previousTime = currentTime;
}
