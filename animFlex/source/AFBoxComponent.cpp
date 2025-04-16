#include "AFBoxComponent.h"

#include "AFUtility.h"

// ------------------------------
// IAFPickerInterface
// ------------------------------

void AFBoxComponent::OnClickPressed(uint8_t elementId)
{
	printf("pressed on: %s\n", GetOwner()->GetDisplayName().c_str());
}

// ------------------------------
// AFStaticMeshComponent
// ------------------------------

bool AFBoxComponent::Load()
{
	AFMesh boxMesh = AFMesh();
	const glm::u8vec4& uniqueIdPacked = AFUtility::PackID(m_uniqueId);

	// Front.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec2(0.0f, 1.0f), uniqueIdPacked); // 0.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec2(0.0f, 0.0f), uniqueIdPacked); // 1.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, 50.0f), glm::vec2(1.0f, 0.0f), uniqueIdPacked); // 2.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec2(1.0f, 1.0f), uniqueIdPacked); // 3.
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(3);

	// Upper.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec2(0.0f, 0.0f), uniqueIdPacked); // 4.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec2(1.0f, 0.0f), uniqueIdPacked); // 5.
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(5);

	// Behind.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec2(0.0f, 1.0f), uniqueIdPacked); // 6.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec2(1.0f, 1.0f), uniqueIdPacked); // 7.
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(4);

	// Under.
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(2);

	// Right.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec2(0.0f, 0.0f), uniqueIdPacked); // 8.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec2(0.0f, 1.0f), uniqueIdPacked); // 9.
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(9);

	// Left.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec2(1.0f, 1.0f), uniqueIdPacked); // 10.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec2(1.0f, 0.0f), uniqueIdPacked); // 11.
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(11);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(0);

	m_mesh = boxMesh;

	return AFStaticMeshComponent::Load();
}
