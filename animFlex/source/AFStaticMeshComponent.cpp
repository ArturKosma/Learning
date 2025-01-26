#include "AFStaticMeshComponent.h"

int AFStaticMeshComponent::GetTrianglesNum()
{
	return m_mesh.vertices.size() / 3;
}

bool AFStaticMeshComponent::Load()
{
	m_vertexBuffer.Init();
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
