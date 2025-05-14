#pragma once
#include "AFStaticMeshComponent.h"

class AFSkeletalMeshComponent : public AFStaticMeshComponent
{
public:

	const std::vector<glm::mat4>& GetJointsMatrices();
};