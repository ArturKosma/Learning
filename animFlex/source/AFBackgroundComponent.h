#pragma once
#include "AFStaticMeshComponent.h"

class AFBackgroundComponent : public AFStaticMeshComponent
{
public:
	AFBackgroundComponent() = default;
	virtual ~AFBackgroundComponent() override = default;

	void Draw(bool override = false, const FAFDrawOverride& overrideProperties = {}) const override;

	std::shared_ptr<AFShader> m_backgroundShader = nullptr;
};