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

void AFUIRenderComponent::SetTexture(const std::string& texturePath)
{
	m_texture.SetTexture(texturePath);
}

bool AFUIRenderComponent::Load()
{
	m_vertexBuffer.Init();
	m_vertexBuffer.UploadMesh(m_mesh);

	if (!m_shader.LoadShaders())
	{
		return false;
	}

	if (!m_texture.LoadTexture())
	{
		return false;
	}

	return true;
}

void AFUIRenderComponent::Draw(const AFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	AFShader drawShader = AFShader();
	switch(overrideProperties.drawType)
	{
		case EAFDrawType::Normal:
		{
			drawShader = m_shader;
			break;
		}
		case EAFDrawType::IDPicker:
		{
			drawShader = overrideProperties.shader;
			break;
		}
		default:
		{
			break;
		}
	}

	m_texture.Bind();

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	drawShader.Use();
	m_vertexBuffer.Bind();

	m_vertexBuffer.Draw(GetDrawMode());

	m_vertexBuffer.UnBind();

	m_texture.UnBind();
}
