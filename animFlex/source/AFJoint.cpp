#include "AFJoint.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>

void AFJoint::SetLocation(const glm::vec3& newLocation)
{
	m_location = newLocation;
}

void AFJoint::SetRotation(const glm::quat newRotation)
{
	m_rotation = newRotation;
}

void AFJoint::SetScale(const glm::vec3& newScale)
{
	m_scale = newScale;
}

glm::vec3 AFJoint::GetLocation() const
{
	return m_location;
}

glm::vec3 AFJoint::GetGlobalLocation() const
{
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(nodeMatrix,
		scale, rotation, translation, skew, perspective);

	return translation;
}

glm::quat AFJoint::GetRotation() const
{
	return m_rotation;
}

glm::quat AFJoint::GetGlobalRotation() const
{
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(nodeMatrix,
		scale, rotation, translation, skew, perspective);

	return rotation;
}

glm::vec3 AFJoint::GetScale() const
{
	return m_scale;
}

std::shared_ptr<AFJoint> AFJoint::CreateRoot(int rootBoneIdx)
{
	std::shared_ptr<AFJoint> parentBone = std::make_shared<AFJoint>();
	parentBone->nodeID = rootBoneIdx;

	return parentBone;
}

void AFJoint::CalculateLocalTRSMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_location);
	glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

	localTRSMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void AFJoint::CalculateNodeMatrix(const glm::mat4& parentNodeMatrix)
{
	nodeMatrix = parentNodeMatrix * localTRSMatrix;
}

void AFJoint::RecalculateBone(const glm::mat4& parentTrs, 
	const std::vector<int>& nodesToJoints, 
	std::vector<glm::mat4>& jointsMatrices,
	const std::vector<glm::mat4>& inverseBindMatrices,
	std::vector<glm::mat4>& jointDualQuats)
{
	CalculateNodeMatrix(parentTrs);
	jointsMatrices.at(nodesToJoints.at(GetNodeID())) = GetNodeMatrix() * inverseBindMatrices.at(nodesToJoints.at(GetNodeID()));

	// Compose dual quat.
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::dualquat dq;
	glm::decompose(GetNodeMatrix() * inverseBindMatrices.at(nodesToJoints.at(GetNodeID())),
		scale, rotation, translation, skew, perspective);

	// Fill dual quaternions. @see C++ Game Animation Programming by Dunsky & Szauer.
	dq[0] = rotation;
	dq[1] = glm::quat(0.0f, translation.x, translation.y, translation.z) * rotation * 0.5f;
	glm::mat2x4 dualQuatJoint = glm::mat2x4_cast(dq);
	glm::mat4 paddedMat(0.0f); // Web-GL doesn't accept 2x4.
	paddedMat[0] = dualQuatJoint[0];
	paddedMat[1] = dualQuatJoint[1];
	jointDualQuats.at(nodesToJoints.at(GetNodeID())) = paddedMat;

	for (std::shared_ptr<AFJoint> childBone : GetChildren())
	{
		childBone->RecalculateBone(nodeMatrix, nodesToJoints, jointsMatrices, inverseBindMatrices, jointDualQuats);
	}
}

glm::mat4 AFJoint::GetLocalTRSMatrix() const
{
	return localTRSMatrix;
}

int AFJoint::GetNodeID() const
{
	return nodeID;
}

void AFJoint::AddChildren(const std::vector<int>& newChildBones)
{
	for (const int childBone : newChildBones)
	{
		std::shared_ptr<AFJoint> child = std::make_shared<AFJoint>();
		child->nodeID = childBone;

		childNodes.push_back(child);
	}
}

void AFJoint::AddChildren(std::shared_ptr<AFJoint> newChildBone)
{
	childNodes.push_back(newChildBone);
}

std::vector<std::shared_ptr<AFJoint>> AFJoint::GetChildren() const
{
	return childNodes;
}

void AFJoint::SetNodeName(const std::string& newName)
{
	nodeName = newName;
}

std::string AFJoint::GetNodeName() const
{
	return nodeName;
}

void AFJoint::PrintTree() const
{
	printf("---- tree ----\n");

	printf("parent : %i (%s)\n", nodeID, nodeName.c_str());

	for (const auto& childBone : childNodes)
	{
		PrintNodes(childBone, 1);
	}

	printf("---- end tree ----\n");
}

void AFJoint::PrintNodes(std::shared_ptr<AFJoint> bone, int indent)
{
	std::string indendString = "";
	for (int i = 0; i < indent; ++i)
	{
		indendString += " ";
	}
	indendString += "-";
	printf("%s child : %i (%s)\n", indendString.c_str(), bone->GetNodeID(), bone->GetNodeName().c_str());

	for (const auto& childNode : bone->childNodes)
	{
		PrintNodes(childNode, indent + 1);
	}
}

glm::mat4 AFJoint::GetNodeMatrix() const
{
	return nodeMatrix;
}

void AFJoint::SetNodeID(int id)
{
	nodeID = id;
}
