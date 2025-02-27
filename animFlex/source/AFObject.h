#pragma once

#include <string>

class AFObject
{
public:

	AFObject() = default;
	virtual ~AFObject() = default;

	virtual void Tick(float deltaTime);

	void SetDisplayName(const std::string& newName);
	std::string GetDisplayName() const;

protected:

	std::string m_displayName = "object";
};
