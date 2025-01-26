#pragma once
#include "AFRenderComponent.h"
#include "AFVertexBuffer.h"

class AFStaticMeshComponent : public AFRenderComponent
{
public:

	int GetTrianglesNum() override;

	bool Load() override;

	void SetShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void SetTexture(const std::string& texturePath);

	void SetMesh(const AFMesh& newMesh);
	const AFMesh& GetMesh() const;

private:

	AFMesh m_mesh = AFMesh();
	AFShader m_shader = AFShader();
	AFTexture m_tex = AFTexture();
	AFVertexBuffer m_vertexBuffer = AFVertexBuffer();
};
