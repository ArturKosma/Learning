#include "AFOrientationGizmoUIComponent.h"

AFOrientationGizmoUIComponent::AFOrientationGizmoUIComponent()
{
	SetDisplayName("Orientation Gizmo UI Component");

	AFMesh orientationGizmoMesh;

	orientationGizmoMesh.vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f)); // 0 - Middle.
	orientationGizmoMesh.vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f)); // 1 - Up.
	orientationGizmoMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f)); // 2 - Right.
	orientationGizmoMesh.vertices.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f)); // 3 - Forward.

	orientationGizmoMesh.indices.emplace_back(0);
	orientationGizmoMesh.indices.emplace_back(1);
	orientationGizmoMesh.indices.emplace_back(0);
	orientationGizmoMesh.indices.emplace_back(2);
	orientationGizmoMesh.indices.emplace_back(0);
	orientationGizmoMesh.indices.emplace_back(3);

	SetMesh(orientationGizmoMesh);

	SetShaders("content/shaders/uiLocalRotateOrtho.vert", "content/shaders/orientationGizmo.frag");
}
