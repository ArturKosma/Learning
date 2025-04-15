#include "AFRenderComponent.h"

AFRenderComponent::AFRenderComponent()
{
}

AFRenderComponent::~AFRenderComponent()
{
}

unsigned long long AFRenderComponent::GetVertexCount()
{
	return 0;
}

bool AFRenderComponent::Load()
{
	return true;
}

void AFRenderComponent::Draw(bool override, const AFDrawOverride& overrideProperties) const
{
}
