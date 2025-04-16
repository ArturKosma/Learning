#pragma once

#include "AFUIRenderComponent.h"
#include "IAFPickerInterface.h"

class AFOrientationBoxUIComponent : public AFUIRenderComponent, public IAFPickerInterface
{
public:
	bool Load() override;

	void Draw(const AFDrawOverride& overrideProperties) const override;

	void OnClickPressed(uint8_t elementId) override;

	GLuint GetDrawMode() const override;

	AFOrientationBoxUIComponent();
	virtual ~AFOrientationBoxUIComponent() = default;

protected:

	AFShader m_shaderIdPick = AFShader();
};
