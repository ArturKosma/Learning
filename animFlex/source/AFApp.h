#pragma once
#include <glm/vec2.hpp>

class AFWindow;
class AFRenderer;
class AFGame;

class AFApp
{
public:

	AFApp();
	~AFApp();

	void StartLoop();
	void CloseApp();

	void OnWindowResize(int newWidth, int newHeight);

	AFGame* GetGame() const;
	AFRenderer* GetRenderer() const;
	AFWindow* GetWindow() const;

	static AFApp& GetInstance();
	static glm::ivec2 GetWindowSize();

	void SetCursorHidden(bool hidden);

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
