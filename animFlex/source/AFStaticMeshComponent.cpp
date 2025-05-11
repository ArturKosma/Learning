#include "AFStaticMeshComponent.h"

#include "AFShader.h"
#include "AFTexture.h"
#include "AFVertexBuffer.h"

void AFStaticMeshComponent::Draw(bool override, const FAFDrawOverride& overrideProperties) const
{
	// Draw every mesh separately.
	for(const FAFSubMesh& sub : m_mesh->subMeshes)
	{
		// Tell the gpu which shader to use.
		override ? overrideProperties.shader->Use() : sub.shader->Use();

		// Bind texture object.
		sub.texture->Bind();

		// Bind the vertex buffer. 
		sub.vertexBuffer->Bind();

		// Should this sub-mesh use stencil testing when drawing.
		if (sub.stencilTest)
		{
			glEnable(GL_STENCIL_TEST);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);

			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		}
		else
		{
			// Disable stencil drawing.
			glDisable(GL_STENCIL_TEST);
			glStencilMask(0x00);
		}

		// Should this sub-mesh use depth testing when drawing.
		if (sub.depthTest)
		{
			// Enable depth.
			glDepthMask(GL_TRUE);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			// Disable depth drawing.
			glDepthMask(GL_FALSE);
			glDisable(GL_DEPTH_TEST);
		}

		// Draw.
		sub.vertexBuffer->Draw();

		// Unbind the buffer with vertices.
		sub.vertexBuffer->UnBind();

		// Unbind the texture object.
		sub.texture->UnBind();
	}
}

void AFStaticMeshComponent::SetMesh(std::shared_ptr<FAFMesh> newMesh)
{
	m_mesh = newMesh;
}

std::shared_ptr<FAFMesh> AFStaticMeshComponent::GetMesh() const
{
	return m_mesh;
}
