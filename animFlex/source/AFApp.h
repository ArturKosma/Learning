#pragma once

#include "AFConfig.h"
#include "AFWindow.h"
#include "AFRenderer.h"
#include "AFGame.h"
#include "AFHelperInterface.h"
#include "AFScene.h"
#include "AFTimerManager.h"

class AFApp
{
public:

	AFApp();
	~AFApp();

	void StartLoop();

	static AFApp& GetInstance();

private:

	void Tick();

	void CollectAppData(AFAppData& appData);

	void SetWindowCallbacks();
	void OnWindowResize(int newWidth, int newHeight);

	AFTimerManager m_timerManager = AFTimerManager();

	AFWindow m_window = AFWindow();
	AFRenderer m_renderer = AFRenderer();
	AFGame m_game = AFGame();
	AFHelperInferface m_helperInterface = AFHelperInferface();
};
