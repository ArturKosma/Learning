#pragma once
#include <vector>

#include "AFComponent.h"
#include "AFObject.h"

class AFComponentOwner : public AFObject
{
public:

	AFComponentOwner() = default;
	virtual ~AFComponentOwner() override = default;

	void Tick(float deltaTime) override;

	void AddComponent(AFComponent* newComponent);
	const std::vector<AFComponent*>& GetComponents() const;

protected:

	std::vector<AFComponent*> m_components = std::vector<AFComponent*>();
};