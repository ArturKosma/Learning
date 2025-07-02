#include "AFPose.h"
#include <chrono>

#include "AFApp.h"
#include "AFContent.h"
#include "AFJoint.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"

AFPose::AFPose()
{
	CreateJoints(AFContent::Get().FindAsset<AFMesh>("sk_mannequin")->GetJoints());
}

void AFPose::CreateJoints(const std::vector<std::shared_ptr<AFJoint>>& joints)
{
	m_joints.clear();
	m_joints.resize(joints.size());

	for (size_t i = 0; i < m_joints.size(); ++i)
	{
		std::shared_ptr<AFJoint> newJoint = std::make_shared<AFJoint>();

		newJoint->SetLocation(joints[i]->GetLocation());
		newJoint->SetRotation(joints[i]->GetRotation());
		newJoint->SetScale(joints[i]->GetScale());

		newJoint->CalculateLocalTRSMatrix();

		m_joints[i] = newJoint;
	}
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
