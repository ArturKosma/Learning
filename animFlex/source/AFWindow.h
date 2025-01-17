#pragma once

class AFWindow
{

public:

	static AFWindow& GetInstance();

	void StartLoop();
	void Tick();

	bool ShouldShutdown();

	AFWindow(const AFWindow&) = delete;
	AFWindow& operator=(const AFWindow&) = delete;
	AFWindow(AFWindow&&) = delete;
	AFWindow& operator=(AFWindow&&) = delete;

private:

	AFWindow();
	~AFWindow();

	void Tick_Internal(float InDeltaTime);

	bool m_WantsShutdown = false;
	float m_DeltaTime = 0.0f;
	double m_PreviousTime = 0.0f;
	struct GLFWwindow* Window = nullptr;
};