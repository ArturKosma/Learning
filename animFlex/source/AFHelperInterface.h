#pragma once
#include "AFStructs.h"

class AFHelperInferface
{
public:

	void Init(const AFAppData& appData);
	void Draw(const AFAppData& appData);
	void Cleanup();

private:

	void CreateFrame(const AFAppData& appData);
	void Render();

	bool m_checkBox_testState = false;
};
