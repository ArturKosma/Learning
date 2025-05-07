#include "AFTextComponent.h"

#include "AFApp.h"
#include "AFTextRender.h"

GLuint AFTextComponent::GetDrawMode() const
{
	return GL_TRIANGLES;
}

void AFTextComponent::SetText(const std::string& text)
{
	m_text = text;
}

std::string AFTextComponent::GetText() const
{
	return m_text;
}

bool AFTextComponent::Load()
{
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
	// Create a mesh for every character.
	// #HACK This is a quick hack. Proper way would be to create an atlas bitmap with all the glyphs.
	std::string::const_iterator c;
	for(c = m_text.begin(); c != m_text.end(); ++c)
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

		// Add the character - mesh pairing.
		AFMesh glyphMesh;
		glyphMesh.vertices.emplace_back(glm::vec3(xpos, ypos + h, 0.0f), glm::vec2(0.0f, 0.0f));
		glyphMesh.vertices.emplace_back(glm::vec3(xpos, ypos, 0.0f), glm::vec2(0.0f, 1.0f));
		glyphMesh.vertices.emplace_back(glm::vec3(xpos + w, ypos, 0.0f), glm::vec2(1.0f, 1.0f));
		glyphMesh.vertices.emplace_back(glm::vec3(xpos + w, ypos + h, 0.0f), glm::vec2(1.0f, 0.0f));
		glyphMesh.indices.emplace_back(0);
		glyphMesh.indices.emplace_back(1);
		glyphMesh.indices.emplace_back(2);
		glyphMesh.indices.emplace_back(0);
		glyphMesh.indices.emplace_back(2);
		glyphMesh.indices.emplace_back(3);
		m_meshes.insert(std::pair<char, AFMesh>(*c, glyphMesh));

		// Bitshift by 6 to get value in pixels (2^6 = 64).
		// @see https://learnopengl.com/In-Practice/Text-Rendering
		xCursor += static_cast<float>(glyph.advance >> 6) * xScale;
	}

	// Set fake texture to prevent texture load fail.
	SetTexture("content/textures/orientBox.png");

	// Set fake mesh to prevent mesh load fail.
	AFMesh fakeMesh;
	fakeMesh.vertices.emplace_back(glm::vec3(0.0f));
	fakeMesh.indices.emplace_back(0);
	SetMesh(fakeMesh);

	return AFUIRenderComponent::Load();
}

void AFTextComponent::Draw(const AFDrawOverride& overrideProperties) const
{
	// Get the loaded characters.
	const std::map<char, FAFGlyph>& characters = AFTextRender::GetInstance().GetCharacters();

	// Activate opengl texture and shader.
	glActiveTexture(GL_TEXTURE0);
	m_shader.Use();

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Enable blending for glyphs.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw every glyph separately.
	// #HACK This is a quick hack. Proper way would be to create an atlas bitmap with all the glyphs.
	std::string::const_iterator c;
	for (c = m_text.begin(); c != m_text.end(); ++c)
	{
		// Find the glyph mesh.
		const AFMesh& glyphMesh = m_meshes.at(*c);

		// Find the required glyph.
		const FAFGlyph& glyph = characters.at(*c);

		// Bind previously generated texture for this glyph.
		glBindTexture(GL_TEXTURE_2D, glyph.textureId);

		// Update & bind VAO.
		const_cast<AFVertexBuffer&>(m_vertexBuffer).UploadMesh(glyphMesh);
		m_vertexBuffer.Bind();

		// Draw.
		m_vertexBuffer.Draw(GetDrawMode());
	}

	// Unbind VAO.
	m_vertexBuffer.UnBind();

	// Unbind texture.
	glBindTexture(GL_TEXTURE_2D, 0);

	// Disable blending for glyphs.
	glDisable(GL_BLEND);
}
