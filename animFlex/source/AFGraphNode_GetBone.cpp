#include "AFGraphNode_GetBone.h"

#include "AFGame.h"
#include "AFJoint.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"
#include "AFUtility.h"

void AFGraphNode_GetBone::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());

	std::shared_ptr<AFMesh> mesh = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetMesh();
	if (!mesh)
	{
		return;
	}

	// Getting all of bone data requires recalculating the current pose.
	// @todo Rethink this. How do we deal with recalculating whole chain on each node to have access to global transform without killing CPU?
	std::shared_ptr<AFJoint> joint = pose.GetJoint("root");
	if (!joint)
	{
		return;
	}
	joint->RecalculateBone(glm::mat4(1.0f), mesh->nodeToJoint, mesh->jointMatrices, mesh->inverseBindMatrices, mesh->jointDualQuats);

	glm::vec3 location = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);

	AFUtility::GetBone(pose,
		m_boneName.GetValue(),
		static_cast<EAFBoneSpace>(m_space.GetValue()),
		location, rotation);

	m_outputLocation.SetValue(location);
	m_outputRotation.SetValue(rotation);
	m_outputPose.SetValue(pose);
}
