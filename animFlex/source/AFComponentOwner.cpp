#include "AFComponentOwner.h"

void AFComponentOwner::Tick(float deltaTime)
{
    for (std::shared_ptr<AFComponent> comp : m_components)
    {
        if (!comp)
        {
            continue;
        }
        comp->Tick(deltaTime);
    }

    AFObject::Tick(deltaTime);
}

void AFComponentOwner::AddComponent(std::shared_ptr<AFComponent> newComponent)
{
    newComponent->SetOwner(this);
    m_components.push_back(newComponent);
}

const std::vector<std::shared_ptr<AFComponent>>& AFComponentOwner::GetComponents() const
{
    return m_components;
}
