#pragma once

#include "AFSceneComponent.h"
#include "AFStructs.h"

class AFRenderComponent : public AFSceneComponent
{
public:

	AFRenderComponent();
	virtual ~AFRenderComponent() override;

	virtual void Draw(bool override = false, const FAFDrawOverride& overrideProperties = {}) const;

};
