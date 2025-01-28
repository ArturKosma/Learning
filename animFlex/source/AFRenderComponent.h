#pragma once

#include "AFComponent.h"

class AFRenderComponent : public AFComponent
{
public:

	AFRenderComponent();
	virtual ~AFRenderComponent() override;

	virtual int GetTrianglesNum();
	virtual bool Load();

	virtual void Draw() const;
};
