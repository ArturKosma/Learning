#include "AFMesh.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "AFContent.h"
#include "AFGLTFLoader.h"
#include "AFVertexBuffer.h"
#include "AFShader.h"

unsigned long long AFSubMesh::GetVertexCount() const
{
	return vertices.size();
}

bool AFMesh::LoadExisting()
{
	for (AFSubMesh& sub : subMeshes)
	{
		if (!sub.vertexBuffer)
		{
			sub.vertexBuffer = std::make_shared<AFVertexBuffer>();
		}
		sub.vertexBuffer->Init();
		sub.vertexBuffer->UploadMesh(sub);
	}

	return true;
}

bool AFMesh::LoadImpl(const char* filepath)
{
	FAFMeshLoaded meshLoaded;

	if (AFGLTFLoader::Load(filepath, meshLoaded))
	{
		// Resize this object's submeshes to match the number of loaded submeshes.
		subMeshes.resize(meshLoaded.subMeshesLoaded.size());

		// Fill the info of every sub-mesh.
		for (int i = 0; i < subMeshes.size(); ++i)
		{
			subMeshes[i].vertexBuffer = std::make_shared<AFVertexBuffer>();
			subMeshes[i].shader = AFContent::Get().FindAsset<AFShader>("shader_basicSkinned"); // #hack Not every mesh is a skinned mesh.
			subMeshes[i].vertexBuffer->UploadMesh(meshLoaded.subMeshesLoaded[i]);
		}

		// Pass bones info. #hack Not every mesh is a skinned mesh.
		rootJoint = meshLoaded.rootJoint;
		nodeToJoint = meshLoaded.nodeToJoint;
		joints = meshLoaded.joints;
		inverseBindMatrices = meshLoaded.inverseBindMatrices;
		jointMatrices = meshLoaded.jointMatrices;
		jointDualQuats = meshLoaded.jointDualQuats;

		return true;
	}

	return false;
}

unsigned long long AFMesh::GetVertexCount() const
{
	unsigned long long vertexCount = 0;
	for (const AFSubMesh& sub : subMeshes)
	{
		vertexCount += sub.GetVertexCount();
	}

	return vertexCount;
}

void AFMesh::RecalculateSkeleton()
{
	RecalculateBone(rootJoint, glm::mat4(1.0f));
	jointsDirty = false;
}

void AFMesh::RecalculateBone(std::shared_ptr<AFJoint> bone, const glm::mat4& parentMatrix)
{
	const int nodeID = bone->GetNodeID();

	bone->CalculateNodeMatrix(parentMatrix);
	jointMatrices.at(nodeToJoint.at(nodeID)) = bone->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(nodeID));

	// Compose dual quat.
	glm::vec3 translation;
	glm::quat rotation;
	glm::vec3 scale;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::dualquat dq;
	glm::decompose(bone->GetNodeMatrix() * inverseBindMatrices.at(nodeToJoint.at(nodeID)),
		scale, rotation, translation, skew, perspective);

	// Fill dual quaternions. @see C++ Game Animation Programming by Dunsky & Szauer.
	dq[0] = rotation;
	dq[1] = glm::quat(0.0f, translation.x, translation.y, translation.z) * rotation * 0.5f;
	glm::mat2x4 dualQuatJoint = glm::mat2x4_cast(dq);
	glm::mat4 paddedMat(0.0f); // Web-GL doesn't accept 2x4.
	paddedMat[0] = dualQuatJoint[0];
	paddedMat[1] = dualQuatJoint[1];
	jointDualQuats.at(nodeToJoint.at(nodeID)) = paddedMat;

	for (std::shared_ptr<AFJoint> childBone : bone->GetChildren())
	{
		RecalculateBone(childBone, bone->GetNodeMatrix());
	}
}

const std::vector<std::shared_ptr<AFJoint>>& AFMesh::GetJoints() const
{
	return joints;
}