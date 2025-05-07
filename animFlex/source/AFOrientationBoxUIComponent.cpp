#include "AFOrientationBoxUIComponent.h"
#include <glm/gtc/type_ptr.inl>

#include "AFCamera.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFTimerManager.h"
#include "AFUtility.h"

void AFOrientationBoxUIComponent::Tick(float deltaTime)
{
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

bool AFOrientationBoxUIComponent::Load()
{
	AFMesh boxMesh = AFMesh();

	// Front.
	glm::u8vec4 idPackedFront = AFUtility::PackID(m_uniqueId, 0);
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), idPackedFront, glm::vec2(0.125f, 0.875f)); // 0.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.75f), glm::vec3(0.0f, 0.0f, 1.0f), idPackedFront, glm::vec2(0.125f, 0.875f)); // 1.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.25f, 0.75f), glm::vec3(0.0f, 0.0f, 1.0f), idPackedFront, glm::vec2(0.125f, 0.875f)); // 2.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.25f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), idPackedFront, glm::vec2(0.125f, 0.875f)); // 3.
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(3);

	// Upper.
	glm::u8vec4 idPackedUpper = AFUtility::PackID(m_uniqueId, 1);
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.25f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), idPackedUpper, glm::vec2(0.375f, 0.875f)); // 8.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.25f, 0.75f), glm::vec3(0.0f, 1.0f, 0.0f), idPackedUpper, glm::vec2(0.375f, 0.875f)); // 9.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.5f, 0.75f), glm::vec3(0.0f, 1.0f, 0.0f), idPackedUpper, glm::vec2(0.375f, 0.875f)); // 10.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), idPackedUpper, glm::vec2(0.375f, 0.875f)); // 11.
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(9);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(11);

	// Behind.
	glm::u8vec4 idPackedBehind = AFUtility::PackID(m_uniqueId, 2);
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.75f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), idPackedBehind, glm::vec2(0.875f, 0.875f)); // 12.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.75f, 0.75f), glm::vec3(0.0f, 0.0f, -1.0f), idPackedBehind, glm::vec2(0.875f, 0.875f)); // 13.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.75f), glm::vec3(0.0f, 0.0f, -1.0f), idPackedBehind, glm::vec2(0.875f, 0.875f)); // 14.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), idPackedBehind, glm::vec2(0.875f, 0.875f)); // 15.
	boxMesh.indices.emplace_back(12);
	boxMesh.indices.emplace_back(13);
	boxMesh.indices.emplace_back(14);
	boxMesh.indices.emplace_back(12);
	boxMesh.indices.emplace_back(14);
	boxMesh.indices.emplace_back(15);

	// Under.
	glm::u8vec4 idPackedUnder = AFUtility::PackID(m_uniqueId, 3);
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.5f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), idPackedUnder, glm::vec2(0.625f, 0.875f)); // 16.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.5f, 0.75f), glm::vec3(0.0f, -1.0f, 0.0f), idPackedUnder, glm::vec2(0.625f, 0.875f)); // 17.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.75f, 0.75f), glm::vec3(0.0f, -1.0f, 0.0f), idPackedUnder, glm::vec2(0.625f, 0.875f)); // 18.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.75f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), idPackedUnder, glm::vec2(0.625f, 0.875f)); // 19.
	boxMesh.indices.emplace_back(16);
	boxMesh.indices.emplace_back(17);
	boxMesh.indices.emplace_back(18);
	boxMesh.indices.emplace_back(16);
	boxMesh.indices.emplace_back(18);
	boxMesh.indices.emplace_back(19);

	// Right.
	glm::u8vec4 idPackedRight = AFUtility::PackID(m_uniqueId, 4);
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.25f, 0.75f), glm::vec3(1.0f, 0.0f, 0.0f), idPackedRight, glm::vec2(0.375f, 0.625f)); // 4.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.25f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), idPackedRight, glm::vec2(0.375f, 0.625f)); // 5.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), idPackedRight, glm::vec2(0.375f, 0.625f)); // 6.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.5f, 0.75f), glm::vec3(1.0f, 0.0f, 0.0f), idPackedRight, glm::vec2(0.375f, 0.625f)); // 7.
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(7);

	// Left.
	glm::u8vec4 idPackedLeft = AFUtility::PackID(m_uniqueId, 5);
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.75f), glm::vec3(-1.0f, 0.0f, 0.0f), idPackedLeft, glm::vec2(0.125f, 0.625f)); // 20.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), idPackedLeft, glm::vec2(0.125f, 0.625f)); // 21.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.25f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), idPackedLeft, glm::vec2(0.125f, 0.625f)); // 22.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.25f, 0.75f), glm::vec3(-1.0f, 0.0f, 0.0f), idPackedLeft, glm::vec2(0.125f, 0.625f)); // 23.
	boxMesh.indices.emplace_back(20);
	boxMesh.indices.emplace_back(21);
	boxMesh.indices.emplace_back(22);
	boxMesh.indices.emplace_back(20);
	boxMesh.indices.emplace_back(22);
	boxMesh.indices.emplace_back(23);

	m_mesh = boxMesh;

	m_shaderIdPick.SetVertexShader("content/shaders/uiLocalRotate.vert");
	m_shaderIdPick.SetFragmentShader("content/shaders/idPicker.frag");

	if (!m_shaderIdPick.LoadShaders())
	{
		return false;
	}

	return AFUIRenderComponent::Load();
}

void AFOrientationBoxUIComponent::Draw(const AFDrawOverride& overrideProperties) const
{
	// Tell the gpu which shader to use.
	AFShader drawShader = AFShader();
	switch (overrideProperties.drawType)
	{
	case EAFDrawType::Normal:
	{
		drawShader = m_shader;
		break;
	}
	case EAFDrawType::IDPicker:
	{
		drawShader = m_shaderIdPick;
		break;
	}
	default:
	{
		break;
	}
	}

	m_texture.Bind();

	// Disable depth for UI drawing.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil for UI drawing.
	glDisable(GL_STENCIL_TEST);
	glStencilMask(0x00);

	drawShader.Use();

	// Send info with brightness for all faces.
	GLuint hoveredLoc = glGetUniformLocation(drawShader.GetProgram(), "uBrightnessFaces");
	glUniform1fv(hoveredLoc, 6, m_brightnessFaces);

	m_vertexBuffer.Bind();

	m_vertexBuffer.Draw(GetDrawMode());

	m_vertexBuffer.UnBind();

	m_texture.UnBind();
}

void AFOrientationBoxUIComponent::OnClickPressed(uint8_t elementId)
{
	AFCamera* currentCamera = AFGame::GetGame()->GetScene().GetActiveCamera();
	if(!currentCamera)
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
			blendTargetLoc = glm::vec3(0.0f, 250.0f, 1000.0f);
			break;
		}
		case 1: // Upper.
		{
			blendTargetLoc = glm::vec3(0.0f, 1000.0f, 0.0f);
			blendTargetRot = glm::vec3(-89.9f, targetYaw, 0.0f);
			break;
		}
		case 2: // Behind.
		{
			blendTargetLoc = glm::vec3(0.0f, 250.0f, -1000.0f);
			blendTargetRot = glm::vec3(0.0f, 180.0f, 0.0f);
			break;
		}
		case 3: // Under.
		{
			blendTargetLoc = glm::vec3(0.0f, -1000.0f, 0.0f);
			blendTargetRot = glm::vec3(89.9f, targetYaw, 0.0f);
			break;
		}
		case 4: // Right.
		{
			blendTargetLoc = glm::vec3(1000.0f, 250.0f, 0.0f);
			blendTargetRot = glm::vec3(0.0f, 90.0f, 0.0f);
			break;
		}
		case 5: // Left.
		{
			blendTargetLoc = glm::vec3(-1000.0f, 250.0f, 0.0f);
			blendTargetRot = glm::vec3(0.0f, -90.0f, 0.0f);
			break;
		}
		default:
		{
			break;
		}
	}

	currentCamera->GetMovementComponent()->BlendTo(blendTargetLoc, blendTargetRot.x, blendTargetRot.y, blendLength);
}

GLuint AFOrientationBoxUIComponent::GetDrawMode() const
{
	return GL_TRIANGLES;
}

AFOrientationBoxUIComponent::AFOrientationBoxUIComponent()
{
	SetDisplayName("Orientation Box UI Component");

	SetTexture("content/textures/orientBox.png");
	SetShaders("content/shaders/uiLocalRotate.vert", "content/shaders/orientationBox.frag");
}
