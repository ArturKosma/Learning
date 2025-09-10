#include "AFGraphNode_GetBone.h"

#include "AFJoint.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"
#include "AFUtility.h"

void AFGraphNode_GetBone::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());

	// Make sure the skeleton is recalculated.
	pose.RecalculateSkeleton();

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
