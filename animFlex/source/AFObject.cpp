#include "AFObject.h"

void AFObject::BeginPlay()
{
}

void AFObject::PreTick(float deltaTime)
{
}

void AFObject::Tick(float deltaTime)
{
}

void AFObject::PostTick(float deltaTime)
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
    return m_uniqueId;
}

const std::vector<unsigned int>& AFObject::GetElementIDs() const
{
    return m_elementIds;
}
