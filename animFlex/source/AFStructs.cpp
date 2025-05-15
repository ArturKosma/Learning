#include "AFStructs.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>

#include "AFContent.h"
#include "AFGLTFLoader.h"
#include "AFUtility.h"
#include "AFVertexBuffer.h"

unsigned int FAFAsset::GetUniqueID() const
{
	return m_uniqueID;
}

bool FAFAsset::LoadExisting()
{
	printf("load existing base\n");
	return false;
}

void AFNode::SetLocation(const glm::vec3& newLocation)
{
	location = newLocation;
}

void AFNode::SetRotation(const glm::quat newRotation)
{
	rotation = newRotation;
}

void AFNode::SetScale(const glm::vec3& newScale)
{
	scale = newScale;
}

glm::vec3 AFNode::GetLocation() const
{
	return location;
}

glm::quat AFNode::GetRotation() const
{
	return rotation;
}

glm::vec3 AFNode::GetScale() const
{
	return scale;
}

std::shared_ptr<AFNode> AFNode::CreateRoot(int rootBoneIdx)
{
	std::shared_ptr<AFNode> parentBone = std::make_shared<AFNode>();
	parentBone->nodeID = rootBoneIdx;

	return parentBone;
}

void AFNode::CalculateLocalTRSMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), location);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	localTRSMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void AFNode::CalculateNodeMatrix(const glm::mat4& parentNodeMatrix)
{
	nodeMatrix = parentNodeMatrix * localTRSMatrix;
}

glm::mat4 AFNode::GetLocalTRSMatrix() const
{
	return localTRSMatrix;
}

int AFNode::GetNodeID() const
{
	return nodeID;
}

void AFNode::AddChildren(const std::vector<int>& newChildBones)
{
	for (const int childBone : newChildBones)
	{
		std::shared_ptr<AFNode> child = std::make_shared<AFNode>();
		child->nodeID = childBone;

		childNodes.push_back(child);
	}
}

std::vector<std::shared_ptr<AFNode>> AFNode::GetChildren() const
{
	return childNodes;
}

void AFNode::SetNodeName(const std::string& newName)
{
	nodeName = newName;
}

std::string AFNode::GetNodeName() const
{
	return nodeName;
}

void AFNode::PrintTree() const
{
	printf("---- tree ----\n");

	printf("parent : %i (%s)\n", nodeID, nodeName.c_str());

	for (const auto& childBone : childNodes)
	{
		PrintNodes(childBone, 1);
	}

	printf("---- end tree ----\n");
}

void AFNode::PrintNodes(std::shared_ptr<AFNode> bone, int indent)
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

glm::mat4 AFNode::GetNodeMatrix() const
{
	return nodeMatrix;
}

unsigned long long FAFSubMesh::GetVertexCount() const
{
	return vertices.size();
}

void FAFMesh::SetJointTransform(int jointIdx, const glm::vec3& newLocation, const glm::quat& newRotation,
	const glm::vec3& newScale)
{
	std::shared_ptr<AFNode> joint = idxToJoint[jointIdx];

	joint->SetLocation(newLocation);
	joint->SetRotation(newRotation);
	joint->SetScale(newScale);

	joint->CalculateLocalTRSMatrix();

	jointsDirty = true;
}

bool FAFMesh::LoadExisting()
{
	for(FAFSubMesh& sub : subMeshes)
	{
		if(!sub.vertexBuffer)
		{
			sub.vertexBuffer = std::make_shared<AFVertexBuffer>();
		}
		sub.vertexBuffer->Init();
		sub.vertexBuffer->UploadMesh(sub);
	}

	return true;
}

bool FAFMesh::LoadImpl(const char* filepath, bool binary)
{
	FAFMeshLoaded meshLoaded;

	if(AFGLTFLoader::Load(filepath, meshLoaded, binary))
	{
		// Resize this object's submeshes to match the number of loaded submeshes.
		subMeshes.resize(meshLoaded.subMeshesLoaded.size());

		// Fill the info of every sub-mesh.
		for(int i = 0; i < subMeshes.size(); ++i)
		{
			subMeshes[i].vertexBuffer = std::make_shared<AFVertexBuffer>();
			subMeshes[i].shader = AFContent::Get().FindAsset<AFShader>("shader_basicSkinned"); // #hack Not every mesh is a skinned mesh.
			subMeshes[i].vertexBuffer->UploadMesh(meshLoaded.subMeshesLoaded[i]);
		}

		// Pass bones info. #hack Not every mesh is a skinned mesh.
		rootJoint = meshLoaded.rootJoint;
		nodeToJoint = meshLoaded.nodeToJoint;
		idxToJoint = meshLoaded.idxToJoint;
		inverseBindMatrices = meshLoaded.inverseBindMatrices;
		jointMatrices = meshLoaded.jointMatrices;
		jointDualQuats = meshLoaded.jointDualQuats;

		return true;
	}

	return false;
}

unsigned long long FAFMesh::GetVertexCount() const
{
	unsigned long long vertexCount = 0;
	for(const FAFSubMesh& sub : subMeshes)
	{
		vertexCount += sub.GetVertexCount();
	}

	return vertexCount;
}

void FAFMesh::RecalculateSkeleton()
{
	RecalculateBone(rootJoint, glm::mat4(1.0f));
	jointsDirty = false;
}

void FAFMesh::RecalculateBone(std::shared_ptr<AFNode> bone, const glm::mat4& parentMatrix)
{
	const int boneID = bone->GetNodeID();

	bone->CalculateNodeMatrix(parentMatrix);
	jointMatrices.at(nodeToJoint.at(boneID)) = bone->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(boneID));

	// Compose dual quat.
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::dualquat dq;
	glm::decompose(bone->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(boneID)),
		scale, rotation, translation, skew, perspective);

	// Fill dual quaternions. @see C++ Game Animation Programming by Dunsky & Szauer.
	dq[0] = rotation;
	dq[1] = glm::quat(0.0f, translation.x, translation.y, translation.z) * rotation * 0.5f;
	glm::mat2x4 dualQuatJoint = glm::mat2x4_cast(dq);
	glm::mat4 paddedMat(0.0f); // Web-GL doesn't accept 2x4.
	paddedMat[0] = dualQuatJoint[0];
	paddedMat[1] = dualQuatJoint[1];
	jointDualQuats.at(nodeToJoint.at(boneID)) = paddedMat;

	for(std::shared_ptr<AFNode> childBone : bone->GetChildren())
	{
		RecalculateBone(childBone, bone->GetNodeMatrix());
	}
}

glm::vec3 FAFMesh::GetJointLocation(int boneIdx) const
{
	return idxToJoint[boneIdx]->GetLocation();
}

glm::quat FAFMesh::GetJointRotation(int boneIdx) const
{
	return idxToJoint[boneIdx]->GetRotation();
}

glm::vec3 FAFMesh::GetJointScale(int boneIdx) const
{
	return idxToJoint[boneIdx]->GetScale();
}
