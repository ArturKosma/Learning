#pragma once

#include "AFFrameBuffer.h"

class AFFramebufferMSUI : public AFFramebuffer
{
public:

	void Bind() override;
	void UnBind() override;

	void DrawToScreen(const FAFSceneData& sceneData) override;
};