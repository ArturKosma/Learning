#include "AFPose.h"
#include <chrono>

#include "AFJoint.h"

void AFPose::ApplyJoints(const std::vector<std::shared_ptr<AFJoint>>& joints)
{
	m_joints = joints;
}

void AFPose::ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time)
{
	if (!(clip))
	{
		return;
	}

	for (auto& channel : clip->GetAnimationChannels())
	{
		const int targetJoint = channel->GetTargetJoint();

		switch (channel->GetTargetPath())
		{
		case EAFTargetPath::Translation:
		{
			m_joints.at(targetJoint)->SetLocation(channel->GetTranslation(time));
			break;
		}
		case EAFTargetPath::Rotation:
		{
			m_joints.at(targetJoint)->SetRotation(channel->GetRotation(time));
			break;
		}
		case EAFTargetPath::Scale:
		{
			m_joints.at(targetJoint)->SetScale(channel->GetScale(time));
			break;
		}
		default:
		{
			break;
		}
		}
	}

	for (auto& joint : m_joints)
	{
		joint->CalculateLocalTRSMatrix();
	}
}

const std::vector<std::shared_ptr<class AFJoint>>& AFPose::GetJoints() const
{
	return m_joints;
}
