#include "AFGridComponent.h"

#include <glad/glad.h>

#include "AFShader.h"
#include "AFVertexBuffer.h"

void AFGridComponent::Draw(bool override, const FAFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	override ? overrideProperties.shader->Use() : m_mesh->subMeshes[0].shader->Use();

	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	//m_mesh->subMeshes[0].texture->Bind();

	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_mesh->subMeshes[0].vertexBuffer->Bind();

	// Enable depth for the grid.
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// Disable stencil for the grid.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	// Draw the triangles.
	m_mesh->subMeshes[0].vertexBuffer->Draw();

	// Unbind the buffer with vertices.
	m_mesh->subMeshes[0].vertexBuffer->UnBind();

	// Unbind the texture object.
	//m_mesh->subMeshes[0].texture->UnBind();
}
