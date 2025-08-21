#include "AFPose.h"
#include <chrono>

#include "AFContent.h"
#include "AFJoint.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"

AFPose::AFPose()
{
	std::shared_ptr<AFMesh> mannequinMesh = AFContent::Get().FindAsset<AFMesh>("sk_mannequin");
	if (mannequinMesh)
	{
		CreateJoints(mannequinMesh->GetJoints());
	}
}

AFPose::AFPose(const AFPose& otherPose)
{
	CreateJoints(AFContent::Get().FindAsset<AFMesh>("sk_mannequin")->GetJoints());
	CopyPoseFrom(otherPose);
}

AFPose& AFPose::operator=(const AFPose& otherPose)
{
	if (this != &otherPose)
	{
		if (m_joints.empty())
		{
			CreateJoints(otherPose.GetJoints());
		}

		CopyPoseFrom(otherPose);
	}

	return *this;
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
		newJoint->SetNodeName(joints[i]->GetNodeName());

		m_joints[i] = newJoint;
	}
}

void AFPose::ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time, bool forceRootLock)
{
	if (!(clip))
	{
		return;
	}

	for (auto& channel : clip->GetAnimationChannels())
	{
		const int targetJoint = channel->GetTargetJoint();

		if (forceRootLock && targetJoint == 0)
		{
			m_joints.at(0)->SetLocation(glm::vec3(0.0f, 0.0f, 0.0f));
			m_joints.at(0)->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			m_joints.at(0)->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

			continue;
		}

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

	for (const auto& [name, curve] : clip->GetCurves())
	{
		m_curvesValues[name] = curve->SampleByTime(time);
	}
}

void AFPose::CopyPoseFrom(const AFPose& otherPose)
{
	assert(m_joints.size() == otherPose.GetJoints().size() && "Incompatible poses");

	const auto& joints = GetJoints();
	const auto& otherJoints = otherPose.GetJoints();

	for (size_t i = 0; i < m_joints.size(); ++i)
	{
		joints[i]->SetLocation(otherJoints[i]->GetLocation());
		joints[i]->SetRotation(otherJoints[i]->GetRotation());
		joints[i]->SetScale(otherJoints[i]->GetScale());

		joints[i]->CalculateLocalTRSMatrix();
	}

	SetCurvesValues(otherPose.m_curvesValues);
}

void AFPose::SetCurvesValues(const std::unordered_map<std::string, float>& curvesValues) const
{
	m_curvesValues = curvesValues;
}

const std::unordered_map<std::string, float>& AFPose::GetCurvesValues() const
{
	return m_curvesValues;
}

const std::vector<std::shared_ptr<class AFJoint>>& AFPose::GetJoints() const
{
	return m_joints;
}
