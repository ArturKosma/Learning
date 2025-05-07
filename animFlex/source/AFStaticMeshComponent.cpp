#include "AFStaticMeshComponent.h"

void AFStaticMeshComponent::Draw(bool override, const AFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	override ? overrideProperties.shader.Use() : m_shader.Use();

	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	m_tex.Bind();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();

	// Should this mesh use stencil testing when drawing.
	if (m_stencilTest)
	{
		glEnable(GL_STENCIL_TEST);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	}

	// Should this mesh use depth testing when drawing.
	if(m_depthTest)
	{
		// Enable depth for rest of scene.
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		// Disable depth for drawing.
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	// Draw the triangles.
	m_vertexBuffer.Draw(GL_TRIANGLES);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();

	// Unbind the texture object.
	m_tex.UnBind();
}

unsigned long long AFStaticMeshComponent::GetVertexCount()
{
	return m_mesh.vertices.size();
}

bool AFStaticMeshComponent::Load()
{
	switch(m_loadSource)
	{
		case EAFLoadSource::Custom:
		{
			m_vertexBuffer.Init();
			m_vertexBuffer.UploadMesh(m_mesh);
			break;
		}
		case EAFLoadSource::VAO:
		{
			break;
		}
	}

	if (!m_tex.LoadTexture())
	{
		return false;
	}

	if (!m_shader.LoadShaders())
	{
		return false;
	}

	return true;
}

void AFStaticMeshComponent::SetShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	m_shader.SetVertexShader(vertexShaderPath);
	m_shader.SetFragmentShader(fragmentShaderPath);
}

void AFStaticMeshComponent::SetTexture(const std::string& texturePath, bool verticallyFlipped)
{
	m_tex.SetTexture(texturePath, verticallyFlipped);
}

void AFStaticMeshComponent::SetMesh(const AFMesh& newMesh)
{
	m_mesh = newMesh;
}

void AFStaticMeshComponent::SetMesh(const AFMeshLoaded& newMesh)
{
	m_loadSource = EAFLoadSource::VAO;
	m_vertexBuffer.UploadMesh(newMesh);
}

const AFMesh& AFStaticMeshComponent::GetMesh() const
{
	return m_mesh;
}

void AFStaticMeshComponent::SetUseDepthTest(bool useDepthTest)
{
	m_depthTest = useDepthTest;
}
