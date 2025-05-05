#pragma once

#include <map>
#include "AFUIRenderComponent.h"

class AFTextComponent : public AFUIRenderComponent
{
public:

	GLuint GetDrawMode() const override;

	void SetText(const std::string& text);
	std::string GetText() const;

	bool Load() override;

	void Draw(const AFDrawOverride& overrideProperties) const override;

protected:

	std::string m_text;
	std::map<char, AFMesh> m_meshes;
};