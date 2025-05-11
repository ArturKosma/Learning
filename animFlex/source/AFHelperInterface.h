#pragma once
#include "AFStructs.h"

class AFHelperInterface
{
public:

	bool Init(const class AFWindow& window);
	void Draw(const FAFSceneData& sceneData, FAFAppData& appData);
	void Cleanup();

private:

	bool CreateFrame(const FAFSceneData& sceneData, FAFAppData& appData);
	void Render();

	bool m_checkBox_testState = false;
};
