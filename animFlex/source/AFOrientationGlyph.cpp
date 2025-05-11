#include "AFOrientationGlyph.h"

#include "AFShader.h"

void AFOrientationGlyph::Draw(const FAFDrawOverride& overrideProperties) const
{
	m_shader->Use();

	int id = 0;
	if(GetText() == "y")
	{
		id = 1;
	}
	else if (GetText() == "z")
	{
		id = 2;
	}

	GLint location = glGetUniformLocation(m_shader->GetProgram(), "aGlyphID");
	glUniform1i(location, id);

	AFTextComponent::Draw(overrideProperties);
}
