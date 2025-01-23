#pragma once
#include "AFRenderData.h"
#include "AFGame.h"

class AFUserInferface
{
public:

	void Init(const AFRenderData& renderData);

	void CreateFrame(AFRenderData& renderData);
	void Render();

	void Cleanup();

private:

	bool m_checkBox_testState = false;
};
