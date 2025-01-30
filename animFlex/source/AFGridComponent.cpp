#include "AFGridComponent.h"

#include <glad/glad.h>

bool AFGridComponent::Load()
{
	m_vertexBuffer.Init(EAFVertexBufferType::Grid);

	AFMesh gridMesh;
	for(int i = -5; i < 5; ++i)
	{
		// X.
		gridMesh.vertices.emplace_back(glm::vec3(static_cast<float>(i), 0.0f, -5.0f));
		gridMesh.vertices.emplace_back(glm::vec3(static_cast<float>(i), 0.0f, 5.0f));
		gridMesh.vertices.emplace_back(glm::vec3(-1.0f * static_cast<float>(i), 0.0f, -5.0f));
		gridMesh.vertices.emplace_back(glm::vec3(-1.0f * static_cast<float>(i), 0.0f, 5.0f));

		// Z.
		gridMesh.vertices.emplace_back(glm::vec3(-5.0f, 0.0f, static_cast<float>(i)));
		gridMesh.vertices.emplace_back(glm::vec3(5.0f, 0.0f, static_cast<float>(i)));
		gridMesh.vertices.emplace_back(glm::vec3(-5.0f, 0.0f, -1.0f * static_cast<float>(i)));
		gridMesh.vertices.emplace_back(glm::vec3(5.0f, 0.0f, -1.0f * static_cast<float>(i)));
	}

	m_mesh = gridMesh;
	m_vertexBuffer.UploadMesh(m_mesh);

	m_shader.SetVertexShader("content/shaders/grid.vert");
	m_shader.SetFragmentShader("content/shaders/grid.frag");

	if (!m_shader.LoadShaders())
	{
		return false;
	}

	return true;
}

void AFGridComponent::Draw() const
{
	// Tell the gpu which shader to use.
	m_shader.Use();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();

	// Draw the triangles.
	m_vertexBuffer.Draw(GL_LINES, 0, m_mesh.vertices.size());

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();
}
