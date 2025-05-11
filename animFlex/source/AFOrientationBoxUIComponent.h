#pragma once

#include "AFUIRenderComponent.h"
#include "IAFPickerInterface.h"

class AFOrientationBoxUIComponent : public AFUIRenderComponent, public IAFPickerInterface
{
public:
	AFOrientationBoxUIComponent();

	void Tick(float deltaTime) override;

	void OnHoverBegin(uint8_t elementId) override;
	void OnHoverEnd(uint8_t elementId) override;

	void Draw(const FAFDrawOverride& overrideProperties) const override;

	void OnClickPressed(uint8_t elementId) override;

	virtual ~AFOrientationBoxUIComponent() override = default;

protected:

	std::shared_ptr<AFShader> m_shader = nullptr;

	float m_brightnessFaces[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
};
