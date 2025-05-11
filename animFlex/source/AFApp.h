#pragma once
#include "AFGame.h"

class AFApp
{
public:

	AFApp();
	~AFApp();

	void Init();
	void StartLoop();

	void OnWindowResize(int newWidth, int newHeight);

	AFGame* GetGame() const;

	static AFApp& GetInstance();
	static glm::ivec2 GetWindowSize();

private:

	void Tick();

	void CollectAppData(struct FAFAppData& appData);
	void SetWindowCallbacks();

	struct FAFAppData* m_appData = nullptr;

	class AFTimerManager* m_timerManager = nullptr;
	class AFWindow* m_window = nullptr;
	class AFRenderer* m_renderer = nullptr;
	class AFGame* m_game = nullptr;
	class AFHelperInterface* m_helperInterface = nullptr;
};
