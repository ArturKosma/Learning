#pragma once

#include <chrono>

class AFTimerManager
{
	friend class AFApp;

public:

	class AFTimer
	{
	public:

		void Start();
		float Stop();

	private:

		bool m_running = false;
		std::chrono::time_point<std::chrono::steady_clock> m_startTime = {};
	};

	static AFTimerManager& GetInstance();
	static float GetDeltaTime();

private:

	void Init();
	void DeltaCalc();

	float m_previousTime = 0.0f;
	float m_deltaTime = 0.0f;
};
