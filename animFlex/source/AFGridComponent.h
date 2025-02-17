#pragma once
#include "AFRenderComponent.h"
#include "AFTexture.h"

class AFGridComponent : public AFRenderComponent
{
public:

	unsigned long long GetVertexCount() override;

	bool Load() override;
	void Draw() const override;

private:

	AFTexture m_tex = AFTexture();
};
