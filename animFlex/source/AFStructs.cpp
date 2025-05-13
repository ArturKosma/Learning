#include "AFStructs.h"

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

void AFBone::SetLocation(const glm::vec3& newLocation)
{
	location = newLocation;
}

void AFBone::SetRotation(const glm::quat newRotation)
{
	rotation = newRotation;
}

void AFBone::SetScale(const glm::vec3& newScale)
{
	scale = newScale;
}

std::shared_ptr<AFBone> AFBone::CreateRoot(int rootBoneIdx)
{
	std::shared_ptr<AFBone> parentBone = std::make_shared<AFBone>();
	parentBone->boneID = rootBoneIdx;

	return parentBone;
}

void AFBone::CalculateLocalTRSMatrix()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), location);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	localTRSMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void AFBone::CalculateNodeMatrix(const glm::mat4& parentNodeMatrix)
{
	boneMatrix = parentNodeMatrix * localTRSMatrix;
}

int AFBone::GetBoneID() const
{
	return boneID;
}

void AFBone::AddChildren(const std::vector<int>& newChildBones)
{
	for (const int childBone : newChildBones)
	{
		std::shared_ptr<AFBone> child = std::make_shared<AFBone>();
		child->boneID = childBone;

		childBones.push_back(child);
	}
}

std::vector<std::shared_ptr<AFBone>> AFBone::GetChildren() const
{
	return childBones;
}

void AFBone::SetBoneName(const std::string& newName)
{
	boneName = newName;
}

std::string AFBone::GetBoneName() const
{
	return boneName;
}

void AFBone::PrintTree() const
{
	printf("---- tree ----\n");

	printf("parent : %i (%s)\n", boneID, boneName.c_str());

	for (const auto& childBone : childBones)
	{
		PrintNodes(childBone, 1);
	}

	printf("---- end tree ----\n");
}

void AFBone::PrintNodes(std::shared_ptr<AFBone> bone, int indent)
{
	std::string indendString = "";
	for (int i = 0; i < indent; ++i)
	{
		indendString += " ";
	}
	indendString += "-";
	printf("%s child : %i (%s)\n", indendString.c_str(), bone->GetBoneID(), bone->GetBoneName().c_str());

	for (const auto& childNode : bone->childBones)
	{
		PrintNodes(childNode, indent + 1);
	}
}

glm::mat4 AFBone::GetBoneMatrix() const
{
	return boneMatrix;
}

unsigned long long FAFSubMesh::GetVertexCount() const
{
	return vertices.size();
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
			subMeshes[i].shader = AFContent::Get().FindAsset<AFShader>("shader_basic");
			subMeshes[i].vertexBuffer->UploadMesh(meshLoaded.subMeshesLoaded[i]);
		}

		// Pass bones info.
		rootbone = meshLoaded.rootbone;

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
