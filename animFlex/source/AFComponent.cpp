#include "AFComponent.h"

void AFComponent::SetOwner(std::weak_ptr<AFObject> newOwner)
{
    m_weakOwner = newOwner;
}

void AFComponent::SetOwner(AFObject* newOwner)
{
	m_rawOwner = newOwner;
}

std::weak_ptr<AFObject> AFComponent::GetOwner() const
{
	return m_weakOwner;
}
