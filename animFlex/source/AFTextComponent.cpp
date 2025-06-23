#include "AFTextComponent.h"

#include "AFApp.h"
#include "AFContent.h"
#include "AFMesh.h"
#include "AFShader.h"
#include "AFTextRender.h"
#include "AFVertexBuffer.h"

AFTextComponent::AFTextComponent()
{
	m_shader = AFContent::Get().FindAsset<AFShader>("shader_glyph");
}

void AFTextComponent::SetText(const std::string& text)
{
	m_mesh = std::make_shared<AFMesh>();
	m_text = text;

	// Get the loaded characters.
	const std::map<char, FAFGlyph>& characters = AFTextRender::GetInstance().GetCharacters();

	// #HACK.
	// Create access to the app's window size during app constructor.
	const float screenWidth = /*static_cast<float>(appData.width)*/ 2560.0f;
	const float screenHeight = /*static_cast<float>(appData.height)*/ 1440.0f;

	const float aspect = screenWidth / screenHeight;
	const float toNDCx = 2.0f * aspect / screenWidth;
	const float toNDCy = 2.0f / screenHeight;

	float xCursor = 0.0f;

	// Iterate through all characters that were set in m_text.
	// Create a sub-mesh for every character.
	// #HACK This is a quick hack. Proper way would be to create an atlas bitmap with all the glyphs.
	std::string::const_iterator c;
	for (c = m_text.begin(); c != m_text.end(); ++c)
	{
		// Find the required glyph.
		const FAFGlyph& glyph = characters.at(*c);

		// Cache scale.
		const float xScale = GetScale().x;
		const float yScale = GetScale().y;

		// Find position for this glyph.
		const float xpos = (xCursor + static_cast<float>(glyph.bearing.x) * xScale) * toNDCx;
		const float ypos = (0.0f - static_cast<float>(glyph.size.y - glyph.bearing.y) * yScale) * toNDCy;

		// Find width and height for this glyph.
		const float w = (static_cast<float>(glyph.size.x) * xScale) * toNDCx;
		const float h = (static_cast<float>(glyph.size.y) * yScale) * toNDCy;

		// Add the character - every glyph is a separate sub-mesh.
		AFSubMesh subMesh;
		subMesh.vertices.emplace_back(glm::vec3(xpos, ypos + h, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 0.0f));
		subMesh.vertices.emplace_back(glm::vec3(xpos, ypos, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f, 1.0f));
		subMesh.vertices.emplace_back(glm::vec3(xpos + w, ypos, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 1.0f));
		subMesh.vertices.emplace_back(glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec3(0.0f), glm::vec2(1.0f, 0.0f));
		subMesh.indices.emplace_back(0);
		subMesh.indices.emplace_back(1);
		subMesh.indices.emplace_back(2);
		subMesh.indices.emplace_back(0);
		subMesh.indices.emplace_back(2);
		subMesh.indices.emplace_back(3);
		subMesh.metaInformation = *c;
		m_mesh->subMeshes.push_back(subMesh);

		// Bitshift by 6 to get value in pixels (2^6 = 64).
		// @see https://learnopengl.com/In-Practice/Text-Rendering
		xCursor += static_cast<float>(glyph.advance >> 6) * xScale;
	}

	m_mesh->LoadExisting();
}

std::string AFTextComponent::GetText() const
{
	return m_text;
}

void AFTextComponent::Draw(const FAFDrawOverride& overrideProperties) const
{
	// Get the loaded characters.
	const std::map<char, FAFGlyph>& characters = AFTextRender::GetInstance().GetCharacters();

	// Activate opengl texture and shader.
	glActiveTexture(GL_TEXTURE0);
	m_shader->Use();

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Enable blending for glyphs.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw every glyph separately.
	// Every glyph is a separate sub-mesh.
	for (std::string::const_iterator c = m_text.begin(); c != m_text.end(); ++c)
	{
		// Find the glyph mesh.
		AFSubMesh glyphSubMesh;
		for(const AFSubMesh& sub : m_mesh->subMeshes)
		{
			if(sub.metaInformation[0] == *c)
			{
				glyphSubMesh = sub;
				break;
			}
		}

		// Find the required glyph.
		const FAFGlyph& glyph = characters.at(*c);

		// Bind previously generated texture for this glyph.
		glBindTexture(GL_TEXTURE_2D, glyph.textureId);

		// Bind VAO of a proper sub-mesh.
		glyphSubMesh.vertexBuffer->Bind();

		// Draw.
		glyphSubMesh.vertexBuffer->Draw();

		// Unbind VAO.
		glyphSubMesh.vertexBuffer->UnBind();

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Disable blending for glyphs.
	glDisable(GL_BLEND);
}
