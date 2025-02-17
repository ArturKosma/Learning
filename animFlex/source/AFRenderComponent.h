#pragma once

#include "AFComponent.h"
#include "AFShader.h"
#include "AFVertexBuffer.h"

class AFRenderComponent : public AFComponent
{
public:

	AFRenderComponent();
	virtual ~AFRenderComponent() override;

	virtual unsigned long long GetVertexCount();
	virtual bool Load();

	virtual void Draw() const;

protected:

	AFVertexBuffer m_vertexBuffer = AFVertexBuffer();
	AFMesh m_mesh = AFMesh();
	AFShader m_shader = AFShader();
};
