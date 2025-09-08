#include "AFComponentOwner.h"

void AFComponentOwner::PreTick(float deltaTime)
{
    for (std::shared_ptr<AFComponent> comp : m_components)
    {
        if (!comp)
        {
            continue;
        }
        comp->PreTick(deltaTime);
    }

    AFObject::PreTick(deltaTime);
}

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

void AFComponentOwner::PostTick(float deltaTime)
{
    for (std::shared_ptr<AFComponent> comp : m_components)
    {
        if (!comp)
        {
            continue;
        }
        comp->PostTick(deltaTime);
    }

    AFObject::PostTick(deltaTime);
}

void AFComponentOwner::AddComponent(std::shared_ptr<AFComponent> newComponent)
{
    // We can either:
    // 1. Add component after the creation of the actor, in that case we set the owner as a smart pointer,
    // or
    // 2. Add component during the construction of the actor, in that case we have to use raw pointers and finalize registering after the construction.
    if (auto w = weak_from_this(); !w.expired()) 
    {
        auto sp = w.lock();
        newComponent->SetOwner(sp);
    }
    else 
    {
        newComponent->SetOwner(this);
    }

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
