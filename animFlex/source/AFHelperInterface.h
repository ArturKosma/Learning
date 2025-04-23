#pragma once
#include "AFStructs.h"

class AFHelperInterface
{
public:

	bool Init(const class AFWindow& window);
	void Draw(const AFSceneData& sceneData, AFAppData& appData);
	void Cleanup();

private:

	bool CreateFrame(const AFSceneData& sceneData, AFAppData& appData);
	void Render();

	bool m_checkBox_testState = false;
};
