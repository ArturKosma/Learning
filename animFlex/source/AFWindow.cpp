#include "AFWindow.h"
#include <thread>

#include <windows.h>

AFWindow& AFWindow::GetInstance()
{
	static AFWindow windowInstance;
	return windowInstance;
}

bool AFWindow::ShouldShutdown()
{
	return m_WantsShutdown;
}

AFWindow::AFWindow()
{
	printf("%s", "window constructed!\n");
	m_LastTimestamp = std::chrono::steady_clock::now();
}

AFWindow::~AFWindow()
{
	printf("%s", "window destructed!\n");
}

void AFWindow::Tick_Internal(float InDeltaTime)
{
	printf("%f\n", 1.0f / InDeltaTime);
}

void AFWindow::Tick()
{
	// Clamp ticks to 60fps ~ 16.667ms.
	constexpr float targetFrameTime = 1.0f / 60.0f;
	constexpr std::chrono::duration<float> targetFrameDuration = std::chrono::duration<float>(targetFrameTime);

	std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
	std::chrono::duration<float> elapsed = now - m_LastTimestamp;

	if (elapsed < targetFrameDuration)
	{
		const std::chrono::duration<float> sleepDuration = targetFrameDuration - elapsed;
		std::this_thread::sleep_for(sleepDuration);

		now = std::chrono::steady_clock::now();
		elapsed = now - m_LastTimestamp;
	}

	m_DeltaTime = elapsed.count();
	m_LastTimestamp = now;

	// Internal tick, doing all the work.
	Tick_Internal(m_DeltaTime);
}
