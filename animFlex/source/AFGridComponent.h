#pragma once
#include "AFRenderComponent.h"

class AFGridComponent : public AFRenderComponent
{
public:
	bool Load() override;
	void Draw() const override;
};