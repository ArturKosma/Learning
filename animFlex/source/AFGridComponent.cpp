#include "AFGridComponent.h"

#include <glad/glad.h>

#include "AFUtility.h"

unsigned long long AFGridComponent::GetVertexCount()
{
	return m_mesh.vertices.size();
}

bool AFGridComponent::Load()
{
	m_vertexBuffer.Init();

	AFMesh gridMesh;

	// Define the flat part.
	// Just 2 triangles.
	gridMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f));
	gridMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	gridMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f));
	gridMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f));
	gridMesh.indices.emplace_back(0);
	gridMesh.indices.emplace_back(1);
	gridMesh.indices.emplace_back(2);
	gridMesh.indices.emplace_back(0);
	gridMesh.indices.emplace_back(2);
	gridMesh.indices.emplace_back(3);

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

void AFGridComponent::Draw(bool override, const AFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	override ? overrideProperties.shader.Use() : m_shader.Use();

	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	m_tex.Bind();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();

	// Enable depth for the grid.
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw the triangles.
	m_vertexBuffer.Draw(GL_TRIANGLES, m_mesh.indices.size());

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();

	// Unbind the texture object.
	m_tex.UnBind();
}
