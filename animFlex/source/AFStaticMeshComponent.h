#pragma once

#include "AFRenderComponent.h"
#include "AFTexture.h"

#include <string>

class AFStaticMeshComponent : public AFRenderComponent
{
public:

	void Draw() const override;

	unsigned long long GetVertexCount() override;

	bool Load() override;

	void SetShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void SetTexture(const std::string& texturePath);

	void SetMesh(const AFMesh& newMesh);
	const AFMesh& GetMesh() const;

	void SetUseDepthTest(bool useDepthTest);

private:

	bool m_depthTest = true;
	AFTexture m_tex = AFTexture();
};
