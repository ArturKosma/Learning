#include "AFOrientationBoxUIComponent.h"
#include "AFUtility.h"

bool AFOrientationBoxUIComponent::Load()
{
	AFMesh boxMesh = AFMesh();

	// Front.
	glm::u8vec4 idPackedFront = AFUtility::PackID(m_uniqueId, 0);
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, 0.96f, 1.0f), glm::vec2(0.0f, 1.0f), idPackedFront); // 0.
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, -0.96f, 1.0f), glm::vec2(0.0f, 0.0f), idPackedFront); // 1.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, -0.96f, 1.0f), glm::vec2(1.0f, 0.0f), idPackedFront); // 2.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, 0.96f, 1.0f), glm::vec2(1.0f, 1.0f), idPackedFront); // 3.
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(3);

	// Upper.
	glm::u8vec4 idPackedUpper = AFUtility::PackID(m_uniqueId, 1);
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, 1.0f, -0.96f), glm::vec2(0.0f, 1.0f), idPackedUpper); // 8.
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, 1.0f, 0.96f), glm::vec2(0.0f, 0.0f), idPackedUpper); // 9.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, 1.0f, 0.96f), glm::vec2(1.0f, 0.0f), idPackedUpper); // 10.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, 1.0f, -0.96f), glm::vec2(1.0f, 1.0f), idPackedUpper); // 11.
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(9);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(11);

	// Behind.
	glm::u8vec4 idPackedBehind = AFUtility::PackID(m_uniqueId, 2);
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, 0.96f, -1.0f), glm::vec2(0.0f, 1.0f), idPackedBehind); // 12.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, -0.96f, -1.0f), glm::vec2(0.0f, 0.0f), idPackedBehind); // 13.
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, -0.96f, -1.0f), glm::vec2(1.0f, 0.0f), idPackedBehind); // 14.
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, 0.96f, -1.0f), glm::vec2(1.0f, 1.0f), idPackedBehind); // 15.
	boxMesh.indices.emplace_back(12);
	boxMesh.indices.emplace_back(13);
	boxMesh.indices.emplace_back(14);
	boxMesh.indices.emplace_back(12);
	boxMesh.indices.emplace_back(14);
	boxMesh.indices.emplace_back(15);

	// Under.
	glm::u8vec4 idPackedUnder = AFUtility::PackID(m_uniqueId, 3);
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, -1.0f, 0.96f), glm::vec2(0.0f, 1.0f), idPackedUnder); // 16.
	boxMesh.vertices.emplace_back(glm::vec3(-0.96f, -1.0f, -0.96f), glm::vec2(0.0f, 0.0f), idPackedUnder); // 17.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, -1.0f, -0.96f), glm::vec2(1.0f, 0.0f), idPackedUnder); // 18.
	boxMesh.vertices.emplace_back(glm::vec3(0.96f, -1.0f, 0.96f), glm::vec2(1.0f, 1.0f), idPackedUnder); // 19.
	boxMesh.indices.emplace_back(16);
	boxMesh.indices.emplace_back(17);
	boxMesh.indices.emplace_back(18);
	boxMesh.indices.emplace_back(16);
	boxMesh.indices.emplace_back(18);
	boxMesh.indices.emplace_back(19);

	// Right.
	glm::u8vec4 idPackedRight = AFUtility::PackID(m_uniqueId, 4);
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 0.96f, 0.96f), glm::vec2(0.0f, 1.0f), idPackedRight); // 4.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -0.96f, 0.96f), glm::vec2(0.0f, 0.0f), idPackedRight); // 5.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, -0.96f, -0.96f), glm::vec2(1.0f, 0.0f), idPackedRight); // 6.
	boxMesh.vertices.emplace_back(glm::vec3(1.0f, 0.96f, -0.96f), glm::vec2(1.0f, 1.0f), idPackedRight); // 7.
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(7);

	// Left.
	glm::u8vec4 idPackedLeft = AFUtility::PackID(m_uniqueId, 5);
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.96f, -0.96f), glm::vec2(0.0f, 1.0f), idPackedLeft); // 20.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -0.96f, -0.96f), glm::vec2(0.0f, 0.0f), idPackedLeft); // 21.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, -0.96f, 0.96f), glm::vec2(1.0f, 0.0f), idPackedLeft); // 22.
	boxMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.96f, 0.96f), glm::vec2(1.0f, 1.0f), idPackedLeft); // 23.
	boxMesh.indices.emplace_back(20);
	boxMesh.indices.emplace_back(21);
	boxMesh.indices.emplace_back(22);
	boxMesh.indices.emplace_back(20);
	boxMesh.indices.emplace_back(22);
	boxMesh.indices.emplace_back(23);

	m_mesh = boxMesh;

	m_shaderIdPick.SetVertexShader("content/shaders/uiLocalRotateOrtho.vert");
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
	m_vertexBuffer.Bind();

	m_vertexBuffer.Draw(GetDrawMode(), m_mesh.indices.size());

	m_vertexBuffer.UnBind();

	m_texture.UnBind();
}

void AFOrientationBoxUIComponent::OnClickPressed(uint8_t elementId)
{
	printf("pressed on orient box, side: %d\n", elementId);
}

GLuint AFOrientationBoxUIComponent::GetDrawMode() const
{
	return GL_TRIANGLES;
}

AFOrientationBoxUIComponent::AFOrientationBoxUIComponent()
{
	SetDisplayName("Orientation Box UI Component");

	SetTexture("content/textures/crate2.png");
	SetShaders("content/shaders/uiLocalRotateOrtho.vert", "content/shaders/basic.frag");
}
