#include "AFBackgroundComponent.h"
#include "AFShader.h"

void AFBackgroundComponent::Draw(bool override, const FAFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	override ? overrideProperties.shader->Use() : m_backgroundShader->Use();

	// Disable depth.
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	// Disable stencil.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw 3 non defined points for the background.
	// Background defines the vertices in background vertex shader.
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
