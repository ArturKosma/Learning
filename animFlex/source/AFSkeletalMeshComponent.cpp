#include "AFSkeletalMeshComponent.h"

const std::vector<glm::mat4>& AFSkeletalMeshComponent::GetJointsMatrices()
{
	return m_mesh->jointMatrices;
}
