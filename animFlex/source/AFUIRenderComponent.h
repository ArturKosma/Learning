#pragma once

#include "AFShader.h"
#include "AFStructs.h"
#include "AFTexture.h"
#include "AFUIComponent.h"
#include "AFVertexBuffer.h"

class AFUIRenderComponent : public AFUIComponent
{
public:

	AFUIRenderComponent() = default;
	virtual ~AFUIRenderComponent() = default;

	virtual GLuint GetDrawMode() const;

	void SetMesh(const AFMesh& newMesh);
	void SetShaders(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void SetTexture(const std::string& texturePath);

	virtual bool Load();

	virtual void Draw(const AFDrawOverride& overrideProperties = {}) const;

protected:

	AFVertexBuffer m_vertexBuffer = AFVertexBuffer();
	AFMesh m_mesh = AFMesh();
	AFShader m_shader = AFShader();
	AFTexture m_texture = AFTexture();
};
