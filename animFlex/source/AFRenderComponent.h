#pragma once

#include "AFComponent.h"
#include "AFShader.h"
#include "AFTexture.h"
#include "AFVertexBuffer.h"

class AFRenderComponent : public AFComponent
{
public:

	AFRenderComponent();
	virtual ~AFRenderComponent() override;

	virtual int GetTrianglesNum();
	virtual bool Load();

	void Draw();
};
