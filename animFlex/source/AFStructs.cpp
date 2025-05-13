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
