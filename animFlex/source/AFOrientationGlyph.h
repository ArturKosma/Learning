#pragma once
#include "AFTextComponent.h"

class AFOrientationGlyph : public AFTextComponent
{
public:

	void Draw(const FAFDrawOverride& overrideProperties) const override;

	std::shared_ptr<AFShader> m_glyphShader = nullptr;
};