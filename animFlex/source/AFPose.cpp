#include "AFPose.h"
#include <chrono>

#include "AFContent.h"
#include "AFGame.h"
#include "AFJoint.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"

AFPose::AFPose()
{
	std::shared_ptr<AFMesh> mannequinMesh = AFContent::Get().FindAsset<AFMesh>("sk_mannequin");
	if (mannequinMesh)
	{
		CreateJoints(mannequinMesh->GetJoints());

		m_nodeToJoint = mannequinMesh->nodeToJoint;
		m_inverseBindMatrices = mannequinMesh->inverseBindMatrices;
		m_jointMatrices = mannequinMesh->inverseBindMatrices;
		m_jointDualQuats = mannequinMesh->jointDualQuats;
	}
}

AFPose::AFPose(const AFPose& otherPose)
{
	if (m_joints.empty())
	{
		CreateJoints(AFContent::Get().FindAsset<AFMesh>("sk_mannequin")->GetJoints());
	}
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

		newJoint->SetLocalMatrix(joints[i]->GetLocalTRSMatrix());
		newJoint->SetGlobalMatrix(joints[i]->GetNodeMatrix());

		newJoint->SetNodeName(joints[i]->GetNodeName());
		newJoint->SetNodeID(joints[i]->GetNodeID());
		newJoint->SetOwnerPose(this);

		m_boneNameToIndex[joints[i]->GetNodeName()] = i;

		m_joints[i] = newJoint;
	}

	// Recreate children connections.
	// For every joint.
	for (size_t i = 0; i < m_joints.size(); ++i)
	{
		const std::vector<std::shared_ptr<AFJoint>>& children = joints[i]->GetChildren();
		if (children.empty())
		{
			continue;
		}

		// Check its children in the input pose.
		// For every child.
		for (const std::shared_ptr<AFJoint>& child : children)
		{
			// Find if such joint exists in this pose.
			auto it = std::find_if(m_joints.begin(), m_joints.end(),
				[child](const std::shared_ptr<AFJoint>& existing)
				{
					return existing->GetNodeID() == child->GetNodeID();
				});
			if (it == m_joints.end())
			{
				continue;
			}

			// If it exists, connect it to this joint.
			m_joints[i]->AddChildren(*it);

			// Inform the child of its parent.
			(*it)->SetParentBone(m_joints[i]);
		}
	}

	for (const std::string& curveName : AFUtility::GetCurveNames())
	{
		m_curvesValues[curveName] = 0.0f;
	}
}

void AFPose::ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time, bool forceRootLock)
{
	if (!clip)
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

	// Recalculate whole skeleton.
	m_joints.at(0)->RecalculateBone(glm::mat4(1.0f), 
		m_nodeToJoint, 
		m_jointMatrices, 
		m_inverseBindMatrices, 
		m_jointDualQuats);

	const auto& curves = clip->GetCurves();
	for (const std::string& curveName : AFUtility::GetCurveNames())
	{
		auto it = curves.find(curveName);
		if (it == curves.end())
		{
			m_curvesValues[curveName] = 0.0f;
			continue;
		}

		m_curvesValues[curveName] = it->second->SampleByTime(time);
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

		joints[i]->SetLocalMatrix(otherJoints[i]->GetLocalTRSMatrix());
		joints[i]->SetGlobalMatrix(otherJoints[i]->GetNodeMatrix());
	}

	SetCurvesValues(otherPose.m_curvesValues);

	m_nodeToJoint = otherPose.m_nodeToJoint;
	m_inverseBindMatrices = otherPose.m_inverseBindMatrices;
	m_jointMatrices = otherPose.m_jointMatrices;
	m_jointDualQuats = otherPose.m_jointDualQuats;
}

void AFPose::SetCurvesValues(const std::unordered_map<std::string, float>& curvesValues) const
{
	m_curvesValues = curvesValues;
}

const std::unordered_map<std::string, float>& AFPose::GetCurvesValues() const
{
	return m_curvesValues;
}

void AFPose::RecalculateSkeleton()
{
	if (m_joints.empty())
	{
		return;
	}

	m_joints[0]->RecalculateBone(glm::mat4(1.0f), m_nodeToJoint, m_jointMatrices, m_inverseBindMatrices, m_jointDualQuats);
}

std::shared_ptr<class AFJoint> AFPose::GetJoint(const std::string& jointName) const
{
	auto itName = m_boneNameToIndex.find(jointName);
	if (itName == m_boneNameToIndex.end())
	{
		return nullptr;
	}

	return m_joints.at(itName->second);
}

const std::vector<std::shared_ptr<class AFJoint>>& AFPose::GetJoints() const
{
	return m_joints;
}
