#include "AFComponentOwner.h"

void AFComponentOwner::Tick(float deltaTime)
{
    for (AFComponent* comp : m_components)
    {
        if (!comp)
        {
            continue;
        }
        comp->Tick(deltaTime);
    }

    AFObject::Tick(deltaTime);
}

void AFComponentOwner::AddComponent(AFComponent* newComponent)
{
    newComponent->SetOwner(this);
    m_components.push_back(newComponent);
}

const std::vector<AFComponent*>& AFComponentOwner::GetComponents() const
{
    return m_components;
}
