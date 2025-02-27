#include "AFComponent.h"

void AFComponent::SetOwner(AFObject* newOwner)
{
    m_owner = newOwner;
}

AFObject* AFComponent::GetOwner() const
{
	return m_owner;
}
