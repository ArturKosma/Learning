#include "AFGraphNode_AddRootYawOffset.h"
#include "AFJoint.h"

void AFGraphNode_AddRootYawOffset::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());
	std::shared_ptr<AFJoint> rootJoint = pose.GetJoints()[0];

	const glm::quat& rot = rootJoint->GetRotation();
	const glm::quat& rotWithOffset = glm::quat(glm::vec3(0.0f, glm::radians(m_offset.GetValue()), 0.0f)) * rot;

	rootJoint->SetRotation(rotWithOffset);
	rootJoint->CalculateLocalTRSMatrix();

	m_outputPose.SetValue(pose);
}
