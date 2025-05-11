#include "AFUIRenderComponent.h"

#include "AFShader.h"
#include "AFTexture.h"
#include "AFVertexBuffer.h"

void AFUIRenderComponent::SetMesh(std::shared_ptr<FAFMesh> newMesh)
{
	m_mesh = newMesh;
}

void AFUIRenderComponent::Draw(const FAFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	std::shared_ptr<AFShader> drawShader = nullptr;
	switch(overrideProperties.drawType)
	{
		case EAFDrawType::Normal:
		{
			drawShader = m_mesh->subMeshes[0].shader;
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

	std::shared_ptr<AFTexture> tex = m_mesh->subMeshes[0].texture;
	if(tex)
	{
		tex->Bind();
	}

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	drawShader->Use();
	m_mesh->subMeshes[0].vertexBuffer->Bind();

	m_mesh->subMeshes[0].vertexBuffer->Draw();

	m_mesh->subMeshes[0].vertexBuffer->UnBind();

	if (tex)
	{
		tex->UnBind();
	}
}
