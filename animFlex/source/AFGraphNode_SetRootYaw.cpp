#include "AFGraphNode_SetRootYaw.h"
#include "AFJoint.h"

void AFGraphNode_SetRootYaw::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());
	std::shared_ptr<AFJoint> rootJoint = pose.GetJoints()[0];

	const glm::quat& rotAroundY = glm::quat(glm::angleAxis(glm::radians(m_yaw.GetValue()), glm::vec3(0.0f, 1.0f, 0.0f)));
	const glm::quat& rotAroundX = glm::quat(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));

	rootJoint->SetRotation(rotAroundY * rotAroundX);
	rootJoint->CalculateLocalTRSMatrix();

	m_outputPose.SetValue(pose);
}
