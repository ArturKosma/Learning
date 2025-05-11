#pragma once

#include "AFUIRenderComponent.h"

class AFTextComponent : public AFUIRenderComponent
{
public:

	AFTextComponent();

	void SetText(const std::string& text);
	std::string GetText() const;

	void Draw(const FAFDrawOverride& overrideProperties) const override;

protected:

	std::string m_text;
	std::shared_ptr<AFShader> m_shader = nullptr;
};