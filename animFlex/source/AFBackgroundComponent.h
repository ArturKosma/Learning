#pragma once
#include "AFStaticMeshComponent.h"

class AFBackgroundComponent : public AFStaticMeshComponent
{
public:
	bool Load() override;

	void Draw() const override;
};