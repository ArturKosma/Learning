#pragma once

#include "AFRenderComponent.h"

class AFStaticMeshComponent : public AFRenderComponent
{
public:

	AFStaticMeshComponent() = default;
	virtual ~AFStaticMeshComponent() override = default;

	void Draw(bool override = false, const FAFDrawOverride& overrideProperties = {}) const override;

	void SetMesh(std::shared_ptr<FAFMesh> newMesh);
	std::shared_ptr<FAFMesh> GetMesh() const;

protected:

	std::shared_ptr<FAFMesh> m_mesh = nullptr;
};
