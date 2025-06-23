#pragma once

#include "AFStructs.h"
#include "AFUIComponent.h"

class AFMesh;

class AFUIRenderComponent : public AFUIComponent
{
public:

	AFUIRenderComponent() = default;
	virtual ~AFUIRenderComponent() = default;

	void SetMesh(std::shared_ptr<AFMesh> newMesh);

	virtual void Draw(const FAFDrawOverride& overrideProperties = {}) const;

protected:

	std::shared_ptr<AFMesh> m_mesh = nullptr;
};
