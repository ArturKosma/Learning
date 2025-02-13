#include "AFGridComponent.h"

#include <glad/glad.h>

#include "AFUtility.h"

bool AFGridComponent::Load()
{
	m_vertexBuffer.Init(EAFVertexBufferType::StaticMesh);

	AFMesh gridMesh;
	/*for(int i = -5; i < 5; ++i)
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
	}*/

	const float singleCellSize = 1.0f;

	gridMesh.vertices.emplace_back(glm::vec3(0.5f, 0.0f, -0.5f) * singleCellSize);
	gridMesh.vertices.emplace_back(glm::vec3(-0.5f, 0.0f, 0.5f) * singleCellSize);
	gridMesh.vertices.emplace_back(glm::vec3(0.5f, 0.0f, 0.5f) * singleCellSize);

	gridMesh.vertices.emplace_back(glm::vec3(0.5f, 0.0f, -0.5f) * singleCellSize);
	gridMesh.vertices.emplace_back(glm::vec3(-0.5f, 0.0f, -0.5f) * singleCellSize);
	gridMesh.vertices.emplace_back(glm::vec3(-0.5f, 0.0f, 0.5f) * singleCellSize);

	gridMesh.vertices[0].color = glm::vec3(1.0f);
	gridMesh.vertices[1].color = glm::vec3(1.0f);
	gridMesh.vertices[2].color = glm::vec3(1.0f);

	gridMesh.vertices[3].color = glm::vec3(1.0f);
	gridMesh.vertices[4].color = glm::vec3(1.0f);
	gridMesh.vertices[5].color = glm::vec3(1.0f);

	gridMesh.vertices[0].uv = glm::vec2(1.0f, 1.0f);
	gridMesh.vertices[1].uv = glm::vec2(0.0f, 0.0f);
	gridMesh.vertices[2].uv = glm::vec2(1.0f, 0.0f);

	gridMesh.vertices[3].uv = glm::vec2(1.0f, 1.0f);
	gridMesh.vertices[4].uv = glm::vec2(0.0f, 1.0f);
	gridMesh.vertices[5].uv = glm::vec2(0.0f, 0.0f);

	m_mesh = gridMesh;
	
	m_vertexBuffer.UploadMesh(m_mesh);

	m_shader.SetVertexShader("content/shaders/grid.vert");
	m_shader.SetFragmentShader("content/shaders/grid.frag");

	if (!m_shader.LoadShaders())
	{
		return false;
	}

	m_tex.SetTexture("content/textures/grid.png");

	if (!m_tex.LoadTexture())
	{
		return false;
	}

	return true;
}

void AFGridComponent::Draw() const
{
	// Tell the gpu which shader to use.
	m_shader.Use();

	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	m_tex.Bind();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();

	// Enable depth for the grid.
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// Draw the triangles.
	m_vertexBuffer.Draw(GL_TRIANGLES, 0, m_mesh.vertices.size());

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();

	// Unbind the texture object.
	m_tex.UnBind();
}
