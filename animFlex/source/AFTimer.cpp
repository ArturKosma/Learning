#include "AFTimer.h"

void AFTimer::Start()
{
	if(m_running)
	{
		return;
	}

	m_running = true;
	m_startTime = std::chrono::steady_clock::now();
}

float AFTimer::Stop()
{
	if(!m_running)
	{
		return 0.0f;
	}

	const auto stopTime = std::chrono::steady_clock::now();
	float elapsed =	std::chrono::duration_cast<std::chrono::microseconds>(stopTime - m_startTime).count() / 1000.0f;

	m_running = false;

	return elapsed;
}
