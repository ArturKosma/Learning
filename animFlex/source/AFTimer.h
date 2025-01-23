#pragma once

#include <chrono>

class AFTimer
{
public:

	void Start();
	float Stop();

private:

	bool m_running = false;
	std::chrono::time_point<std::chrono::steady_clock> m_startTime = {};
};