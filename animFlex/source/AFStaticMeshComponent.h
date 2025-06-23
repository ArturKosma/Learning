#pragma once

#include "AFRenderComponent.h"

class AFMesh;

class AFStaticMeshComponent : public AFRenderComponent
{
public:

	AFStaticMeshComponent() = default;
	virtual ~AFStaticMeshComponent() override = default;

	void Draw(bool override = false, const FAFDrawOverride& overrideProperties = {}) const override;

	void SetMesh(std::shared_ptr<AFMesh> newMesh);
	std::shared_ptr<AFMesh> GetMesh() const;

protected:

	std::shared_ptr<AFMesh> m_mesh = nullptr;
};
