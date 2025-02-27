#pragma once
#include "AFObject.h"

class AFComponent : public AFObject
{
public:

	AFComponent() = default;
	virtual ~AFComponent() = default;

	void SetOwner(class AFObject* newOwner);
	AFObject* GetOwner() const;

protected:

	AFObject* m_owner = nullptr;
};
