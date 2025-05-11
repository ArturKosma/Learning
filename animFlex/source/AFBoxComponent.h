#pragma once
#include "AFStaticMeshComponent.h"
#include "IAFPickerInterface.h"

class AFBoxComponent : public AFStaticMeshComponent, public IAFPickerInterface
{
public:

	void OnClickPressed(uint8_t elementId) override;
	void OnHoverBegin(uint8_t elementId) override;
	void OnHoverEnd(uint8_t elementId) override;
};
