#include "AFDebugShapeActor.h"

#include "AFContent.h"
#include "AFMesh.h"
#include "AFScene.h"
#include "AFStaticMeshComponent.h"
#include "AFVertexBuffer.h"
#include "AFShader.h"
#include "AFTexture.h"

void AFDebugShapeActor::Tick(float deltaTime)
{
	m_lifetime -= deltaTime;
	if (m_lifetime <= 0.0f)
	{
		MarkForDestruction();
	}
}

void AFDebugShapeActor::AssignDebugShape(EAFDebugShape shape, EAFColor color)
{
	std::shared_ptr<AFStaticMeshComponent> meshComp = AFScene::CreateObject<AFStaticMeshComponent>();
	m_mesh = std::make_shared<AFMesh>();
	AFSubMesh subMesh = AFSubMesh();

	FillVertices(shape, subMesh);
	SetColor(subMesh, color);

	m_mesh->subMeshes.push_back(subMesh);
	m_mesh->LoadExisting();
	m_mesh->subMeshes[0].vertexBuffer->SetDrawMode(GL_LINES);
	m_mesh->subMeshes[0].shader = AFContent::Get().FindAsset<AFShader>("shader_basicNoTex");

	meshComp->SetMesh(m_mesh);
	AddComponent(meshComp);
}

void AFDebugShapeActor::SetLifetime(float lifetime)
{
	m_lifetime = lifetime;
}

float AFDebugShapeActor::GetLifetime() const
{
	return m_lifetime;
}

void AFDebugShapeActor::SetColor(AFSubMesh& subMesh, EAFColor color)
{
	for (FAFVertex& vertex : subMesh.vertices)
	{
		vertex.color = AFUtility::GetColorVector(color);
	}
}

void AFDebugShapeActor::FillVertices(EAFDebugShape shape, AFSubMesh& subMesh)
{
	switch (shape)
	{
		case EAFDebugShape::Box:
		{
			subMesh.vertices.emplace_back(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)); // 0.
			subMesh.vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)); // 1.
			subMesh.vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)); // 2.
			subMesh.vertices.emplace_back(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)); // 3.
			subMesh.vertices.emplace_back(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)); // 4.
			subMesh.vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)); // 5.
			subMesh.vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)); // 6.
			subMesh.vertices.emplace_back(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)); // 7.
			subMesh.indices.emplace_back(0);
			subMesh.indices.emplace_back(1);
			subMesh.indices.emplace_back(1);
			subMesh.indices.emplace_back(2);
			subMesh.indices.emplace_back(2);
			subMesh.indices.emplace_back(3);
			subMesh.indices.emplace_back(3);
			subMesh.indices.emplace_back(0);
			subMesh.indices.emplace_back(4);
			subMesh.indices.emplace_back(5);
			subMesh.indices.emplace_back(5);
			subMesh.indices.emplace_back(6);
			subMesh.indices.emplace_back(6);
			subMesh.indices.emplace_back(7);
			subMesh.indices.emplace_back(7);
			subMesh.indices.emplace_back(4);

			subMesh.indices.emplace_back(0);
			subMesh.indices.emplace_back(4);
			subMesh.indices.emplace_back(1);
			subMesh.indices.emplace_back(5);
			subMesh.indices.emplace_back(2);
			subMesh.indices.emplace_back(6);
			subMesh.indices.emplace_back(3);
			subMesh.indices.emplace_back(7);

			break;
		}

		default:
		{
			break;
		}
	}
}
