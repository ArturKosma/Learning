#pragma once
#include "AFStructs.h"

class AFHelperInferface
{
public:

	bool Init(const class AFWindow& window);
	void Draw(const AFAppData& appData, const AFSceneData& sceneData);
	void Cleanup();

private:

	bool CreateFrame(const AFAppData& appData, const AFSceneData& sceneData);
	void Render();

	bool m_checkBox_testState = false;
};
