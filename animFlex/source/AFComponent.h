#pragma once
#include <memory>
#include "AFObject.h"

class AFComponent : public AFObject
{
public:

	AFComponent() = default;
	virtual ~AFComponent() = default;

	void SetOwner(std::weak_ptr<AFObject> newOwner);
	void SetOwner(AFObject* newOwner);
	std::weak_ptr<AFObject> GetOwner() const;

protected:

	// Used to temporarily register owner of the component during the construction of the owner.
	// Owners are being created as shared_ptrs so until their construction is finished I can't register the owner as smart ptr.
	AFObject* m_rawOwner = nullptr;
	std::weak_ptr<AFObject> m_weakOwner = {};
};
