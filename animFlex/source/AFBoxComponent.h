#pragma once
#include "AFStaticMeshComponent.h"
#include "IAFPickerInterface.h"

class AFBoxComponent : public AFStaticMeshComponent, public IAFPickerInterface
{
public:

	// ------------------------------
	// IAFPickerInterface
	// ------------------------------

	void OnClickPressed() override;
	void OnClickReleased() override;

	// ------------------------------
	// AFStaticMeshComponent
	// ------------------------------

	bool Load() override;
};
