#pragma once
#include <memory>
#include <vector>

#include "AFComponent.h"
#include "AFObject.h"

class AFComponentOwner : public AFObject
{
public:

	AFComponentOwner() = default;
	virtual ~AFComponentOwner() override = default;

	void Tick(float deltaTime) override;

	void AddComponent(std::shared_ptr<AFComponent> newComponent);

	const std::vector<std::shared_ptr<AFComponent>>& GetComponents() const;
	std::shared_ptr<AFComponent> GetComponentByName(const std::string& compName) const;

	template<typename T>
	std::shared_ptr<T> GetComponentByType();

	void RegisterComponents();

protected:

	std::vector<std::shared_ptr<AFComponent>> m_components = {};
};

template <typename T>
std::shared_ptr<T> AFComponentOwner::GetComponentByType()
{
	for (std::shared_ptr<AFComponent> comp : m_components)
	{
		std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(comp);
		if (casted)
		{
			return casted;
		}
	}

	return nullptr;
}
