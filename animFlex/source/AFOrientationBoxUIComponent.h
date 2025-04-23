#pragma once

#include "AFUIRenderComponent.h"
#include "IAFPickerInterface.h"

class AFOrientationBoxUIComponent : public AFUIRenderComponent, public IAFPickerInterface
{
public:
	void Tick(float deltaTime) override;

	void OnHoverBegin(uint8_t elementId) override;
	void OnHoverEnd(uint8_t elementId) override;

	bool Load() override;

	void Draw(const AFDrawOverride& overrideProperties) const override;

	void OnClickPressed(uint8_t elementId) override;

	GLuint GetDrawMode() const override;

	AFOrientationBoxUIComponent();
	virtual ~AFOrientationBoxUIComponent() = default;

protected:

	AFShader m_shaderIdPick = AFShader();

	float m_brightnessFaces[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
};
