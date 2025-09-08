#include "AFOrientationBoxUIComponent.h"
#include <glm/gtc/type_ptr.inl>

#include "AFApp.h"
#include "AFCamera.h"
#include "AFContent.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFMesh.h"
#include "AFTexture.h"
#include "AFTimerManager.h"
#include "AFUtility.h"
#include "AFVertexBuffer.h"

std::vector<uint8_t> AFOrientationBoxUIComponent::GetDisabledElements()
{
	return { 3 };
}

AFOrientationBoxUIComponent::AFOrientationBoxUIComponent()
{
	m_shader = AFContent::Get().FindAsset<AFShader>("shader_orientBox");
}

void AFOrientationBoxUIComponent::Tick(float deltaTime)
{
	glm::ivec2 dims = AFApp::GetWindowSize();
	const float counterScale = AFMath::MapRangeClamped(static_cast<float>(dims.y), 300.0f, 1000.0f, 0.002f, 0.001f); // #hack. Scale up the orient box when window get smaller.

	SetScale({ counterScale, counterScale });

	AFUIRenderComponent::Tick(deltaTime);
}

void AFOrientationBoxUIComponent::OnHoverBegin(uint8_t elementId)
{
	AFTimerManager::SetAlphaTimer([this, elementId](float alpha, float timeElapsed)
		{
			alpha = AFMath::MapRangeClamped(alpha, 0.0f, 1.0f, 1.0f, 1.2f);
			m_brightnessFaces[elementId] = glm::max(alpha, m_brightnessFaces[elementId]);
		}, 0.2f);
}

void AFOrientationBoxUIComponent::OnHoverEnd(uint8_t elementId)
{
	AFTimerManager::SetAlphaTimer([this, elementId](float alpha, float timeElapsed)
		{
			alpha = AFMath::MapRangeClamped(glm::abs(alpha - 1.0f), 0.0f, 1.0f, 1.0f, 1.2f);
			m_brightnessFaces[elementId] = glm::min(alpha, m_brightnessFaces[elementId]);
		}, 0.2f);
}

void AFOrientationBoxUIComponent::Draw(const FAFDrawOverride& overrideProperties) const
{
	std::shared_ptr<AFShader> drawShader = nullptr;
	switch (overrideProperties.drawType)
	{
	case EAFDrawType::Normal:
	{
		drawShader = m_shader;
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

	m_mesh->subMeshes[0].texture->Bind();

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	drawShader->Use();

	// Send info with brightness for all faces.
	GLuint hoveredLoc = glGetUniformLocation(drawShader->GetProgram(), "uBrightnessFaces");
	glUniform1fv(hoveredLoc, 6, m_brightnessFaces);

	m_mesh->subMeshes[0].vertexBuffer->Bind();

	m_mesh->subMeshes[0].vertexBuffer->Draw();

	m_mesh->subMeshes[0].vertexBuffer->UnBind();

	m_mesh->subMeshes[0].texture->UnBind();
}

void AFOrientationBoxUIComponent::OnClickPressed(uint8_t elementId)
{
	AFCameraManager* cameraManager = AFGame::GetGame()->GetCameraManager();
	if(!cameraManager)
	{
		return;
	}

	std::shared_ptr<AFCamera> currentCamera = std::dynamic_pointer_cast<AFCamera>(cameraManager->GetActiveCamera()->GetOwner().lock());
	if (!currentCamera)
	{
		return;
	}

	const float controlYaw = currentCamera->GetMovementComponent()->GetControlRotation().y;
	const float targetYaw = glm::round(controlYaw / 90.0f) * 90.0f;

	const float blendLength = 0.45f;

	glm::vec3 blendTargetLoc = {};
	glm::vec3 blendTargetRot = {};

	switch(elementId)
	{
		case 0: // Front.
		{
			blendTargetLoc = glm::vec3(0.0f, 120.0f, 300.0f);
			break;
		}
		case 1: // Upper.
		{
			blendTargetLoc = glm::vec3(0.0f, 300.0f, 0.0f);
			blendTargetRot = glm::vec3(-89.9f, targetYaw, 0.0f);
			break;
		}
		case 2: // Behind.
		{
			blendTargetLoc = glm::vec3(0.0f, 120.0f, -300.0f);
			blendTargetRot = glm::vec3(0.0f, 180.0f, 0.0f);
			break;
		}
		case 3: // Under.
		{
			blendTargetLoc = glm::vec3(0.0f, -300.0f, 0.0f);
			blendTargetRot = glm::vec3(89.9f, targetYaw, 0.0f);
			break;
		}
		case 4: // Right.
		{
			blendTargetLoc = glm::vec3(300.0f, 120.0f, 0.0f);
			blendTargetRot = glm::vec3(0.0f, 90.0f, 0.0f);
			break;
		}
		case 5: // Left.
		{
			blendTargetLoc = glm::vec3(-300.0f, 120.0f, 0.0f);
			blendTargetRot = glm::vec3(0.0f, -90.0f, 0.0f);
			break;
		}
		default:
		{
			break;
		}
	}

	cameraManager->BlendToStatic(blendTargetLoc, blendTargetRot.x, blendTargetRot.y, blendLength);
}
