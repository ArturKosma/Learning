#pragma once
#include "AFTextComponent.h"

class AFOrientationGlyph : public AFTextComponent
{
public:

	void Draw(const AFDrawOverride& overrideProperties) const override;
};