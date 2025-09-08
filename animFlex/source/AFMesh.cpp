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
	rootJoint->RecalculateBone(glm::mat4(1.0f), nodeToJoint, jointMatrices, inverseBindMatrices, jointDualQuats);
	jointsDirty = false;
}

const std::vector<std::shared_ptr<AFJoint>>& AFMesh::GetJoints() const
{
	return joints;
}

glm::vec3 AFMesh::GetGlobalJointLocation(int index) const
{
	return joints[index]->GetGlobalLocation();
}

glm::quat AFMesh::GetGlobalJointRotation(int index) const
{
	return joints[index]->GetGlobalRotation();
}
