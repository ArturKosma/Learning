#pragma once

#include <memory>
#include <string>
#include <vector>

class AFObject : public std::enable_shared_from_this<AFObject>
{
	friend class AFScene;

public:

	AFObject() = default;
	virtual ~AFObject() = default;

	virtual void BeginPlay();

	virtual void PreTick(float deltaTime);
	virtual void Tick(float deltaTime);
	virtual void PostTick(float deltaTime);

	void SetDisplayName(const std::string& newName);
	std::string GetDisplayName() const;

	unsigned int GetUniqueID() const;
	const std::vector<unsigned int>& GetElementIDs() const;

protected:

	unsigned int m_uniqueId = 0;
	std::vector<unsigned int> m_elementIds = {};

	std::string m_displayName = "object";
};
