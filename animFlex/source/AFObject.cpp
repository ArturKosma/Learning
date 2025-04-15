#include "AFObject.h"

void AFObject::Tick(float deltaTime)
{
}

void AFObject::SetDisplayName(const std::string& newName)
{
    m_displayName = newName;
}

std::string AFObject::GetDisplayName() const
{
    return m_displayName;
}

unsigned int AFObject::GetUniqueID() const
{
    return uniqueId;
}
