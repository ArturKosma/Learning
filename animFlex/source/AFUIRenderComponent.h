#pragma once

#include "AFStructs.h"
#include "AFUIComponent.h"

class AFUIRenderComponent : public AFUIComponent
{
public:

	AFUIRenderComponent() = default;
	virtual ~AFUIRenderComponent() = default;

	void SetMesh(std::shared_ptr<FAFMesh> newMesh);

	virtual void Draw(const FAFDrawOverride& overrideProperties = {}) const;

protected:

	std::shared_ptr<FAFMesh> m_mesh = nullptr;
};
