#pragma once

#include <string>
#include <vector>

class AFObject
{
	friend class AFScene;

public:

	AFObject() = default;
	virtual ~AFObject() = default;

	virtual void Tick(float deltaTime);

	void SetDisplayName(const std::string& newName);
	std::string GetDisplayName() const;

	unsigned int GetUniqueID() const;
	const std::vector<unsigned int>& GetElementIDs() const;

protected:

	unsigned int m_uniqueId = 0;
	std::vector<unsigned int> m_elementIds = {};

	std::string m_displayName = "object";
};
