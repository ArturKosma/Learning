#include "AFStaticMeshComponent.h"

void AFStaticMeshComponent::Draw() const
{
	// Tell the gpu which shader to use.
	m_shader.Use();

	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	m_tex.Bind();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();

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
	m_vertexBuffer.Draw(GL_TRIANGLES, 0, m_mesh.vertices.size());

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();

	// Unbind the texture object.
	m_tex.UnBind();
}

int AFStaticMeshComponent::GetTrianglesNum()
{
	return static_cast<int>(m_mesh.vertices.size() / 3);
}

bool AFStaticMeshComponent::Load()
{
	m_vertexBuffer.Init(EAFVertexBufferType::StaticMesh);
	m_vertexBuffer.UploadMesh(m_mesh);

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

void AFStaticMeshComponent::SetTexture(const std::string& texturePath)
{
	m_tex.SetTexture(texturePath);
}

void AFStaticMeshComponent::SetMesh(const AFMesh& newMesh)
{
	m_mesh = newMesh;
}

const AFMesh& AFStaticMeshComponent::GetMesh() const
{
	return m_mesh;
}

void AFStaticMeshComponent::SetUseDepthTest(bool useDepthTest)
{
	m_depthTest = useDepthTest;
}
