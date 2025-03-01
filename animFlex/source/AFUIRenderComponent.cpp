#include "AFUIRenderComponent.h"

GLuint AFUIRenderComponent::GetDrawMode() const
{
	return GL_LINES;
}

void AFUIRenderComponent::SetMesh(const AFMesh& newMesh)
{
	m_mesh = newMesh;
}

void AFUIRenderComponent::SetShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	m_shader.SetVertexShader(vertexShaderPath);
	m_shader.SetFragmentShader(fragmentShaderPath);
}

bool AFUIRenderComponent::Load()
{
	m_vertexBuffer.Init();
	m_vertexBuffer.UploadMesh(m_mesh);

	if (!m_shader.LoadShaders())
	{
		return false;
	}

	return true;
}

void AFUIRenderComponent::Draw() const
{
	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	m_shader.Use();
	m_vertexBuffer.Bind();

	m_vertexBuffer.Draw(GetDrawMode(), m_mesh.indices.size());

	m_vertexBuffer.UnBind();
}
