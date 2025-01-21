#pragma once
#include "AFRenderData.h"

class AFUserInferface
{
public:

	void Init(const AFRenderData& renderData);

	void CreateFrame(const AFRenderData& renderData);
	void Render();

	void Cleanup();
};
