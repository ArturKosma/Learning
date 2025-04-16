#pragma once
#include "AFStaticMeshComponent.h"
#include "IAFPickerInterface.h"

class AFBoxComponent : public AFStaticMeshComponent, public IAFPickerInterface
{
public:

	// ------------------------------
	// IAFPickerInterface
	// ------------------------------

	void OnClickPressed(uint8_t elementId) override;

	// ------------------------------
	// AFStaticMeshComponent
	// ------------------------------

	bool Load() override;
};
