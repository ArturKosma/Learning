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

	// How many vertices in a single grid line row.
	// Grid is constructed in a way that whole model space is squeezed into a <-0.5f, 0.5f> space.
	// Later on the vertex shader scales the grid
	const int gridComplexity = 100;

	for(int i = 0; i < gridComplexity; ++i)
	{
		for(int j = 0; j < gridComplexity; ++j)
		{
			const glm::vec3 anchor = glm::vec3((static_cast<float>(j) / gridComplexity) - 0.5f, 0.0f, (static_cast<float>(i) / gridComplexity) - 0.5f);
			const float offset = 1.0f / gridComplexity;

			// Two triangles make a square.
			const glm::vec3 vertex0 = glm::vec3(anchor.x, 0.0f, anchor.z);
			const glm::vec3 vertex1 = glm::vec3(anchor.x, 0.0f, anchor.z + offset);
			const glm::vec3 vertex2 = glm::vec3(anchor.x + offset, 0.0f, anchor.z);
			const glm::vec3 vertex3 = glm::vec3(anchor.x + offset, 0.0f, anchor.z);
			const glm::vec3 vertex4 = glm::vec3(anchor.x, 0.0f, anchor.z + offset);
			const glm::vec3 vertex5 = glm::vec3(anchor.x + offset, 0.0f, anchor.z + offset);

			const glm::vec2 uv = glm::vec2(anchor.x + 0.5f, anchor.z + 0.5f);

			const glm::vec2 uv0 = glm::vec2(uv.x, uv.y + offset);
			const glm::vec2 uv1 = glm::vec2(uv.x, uv.y);
			const glm::vec2 uv2 = glm::vec2(uv.x + offset, uv.y + offset);
			const glm::vec2 uv3 = glm::vec2(uv.x + offset, uv.y + offset);
			const glm::vec2 uv4 = glm::vec2(uv.x, uv.y);
			const glm::vec2 uv5 = glm::vec2(uv.x + offset, uv.y);

			gridMesh.vertices.emplace_back(vertex0, glm::vec3(0.0f), uv0);
			gridMesh.vertices.emplace_back(vertex1, glm::vec3(0.0f), uv1);
			gridMesh.vertices.emplace_back(vertex2, glm::vec3(0.0f), uv2);
			gridMesh.vertices.emplace_back(vertex3, glm::vec3(0.0f), uv3);
			gridMesh.vertices.emplace_back(vertex4, glm::vec3(0.0f), uv4);
			gridMesh.vertices.emplace_back(vertex5, glm::vec3(0.0f), uv5);
		}
	}

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
	//m_vertexBuffer.Draw(GL_LINES, 0, m_mesh.vertices.size());

	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();

	// Unbind the texture object.
	m_tex.UnBind();
}
