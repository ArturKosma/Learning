#include "AFBackgroundComponent.h"

bool AFBackgroundComponent::Load()
{
	// Set up the background shader.
	m_shader.SetVertexShader("content/shaders/background.vert");
	m_shader.SetFragmentShader("content/shaders/background.frag");
	if (!m_shader.LoadShaders())
	{
		printf("%s\n", "Fail on background shader load.");
		return false;
	}

	return true;
}

void AFBackgroundComponent::Draw(bool override, const AFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	override ? overrideProperties.shader.Use() : m_shader.Use();

	// Disable depth for background drawing.
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw 3 non defined points for the background.
	// Background defines the vertices in background vertex shader.
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
