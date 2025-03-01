#pragma once
#include "AFStaticMeshComponent.h"

class AFBackgroundComponent : public AFStaticMeshComponent
{
public:
	AFBackgroundComponent() = default;
	virtual ~AFBackgroundComponent() override = default;

	bool Load() override;

	void Draw() const override;
};