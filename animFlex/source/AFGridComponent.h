#pragma once
#include "AFStaticMeshComponent.h"

class AFGridComponent : public AFStaticMeshComponent
{
public:

	void Draw(bool override = false, const FAFDrawOverride& overrideProperties = {}) const override;
};
