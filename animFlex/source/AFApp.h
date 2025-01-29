#pragma once

class AFApp
{
public:

	AFApp();
	~AFApp();

	void StartLoop();
	void OnWindowResize(int newWidth, int newHeight);

	static AFApp& GetInstance();

private:

	void Tick();

	void CollectAppData(struct AFAppData& appData);

	void SetWindowCallbacks();

	class AFTimerManager* m_timerManager = nullptr;
	class AFWindow* m_window = nullptr;
	class AFRenderer* m_renderer = nullptr;
	class AFGame* m_game = nullptr;
	class AFHelperInferface* m_helperInterface = nullptr;
};
