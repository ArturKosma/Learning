#pragma once

#include <string>

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

protected:

	unsigned int uniqueId = 0;
	std::string m_displayName = "object";
};
