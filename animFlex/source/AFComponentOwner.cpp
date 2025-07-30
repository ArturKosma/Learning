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

std::shared_ptr<AFComponent> AFComponentOwner::GetComponentByName(const std::string& compName) const
{
    auto it = std::find_if(m_components.begin(), m_components.end(),
        [compName](std::shared_ptr<AFComponent> comp)
        {
            return comp && comp->GetDisplayName() == compName;
        });

    if (it != m_components.end())
    {
        return *it;
    }

    return nullptr;
}

void AFComponentOwner::RegisterComponents()
{
    for (std::shared_ptr<AFComponent> comp : m_components)
    {
        comp->SetOwner(shared_from_this());
    }
}
