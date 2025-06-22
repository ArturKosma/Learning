#pragma once

#include "AFGraphNodeRegistry.h"

class AFGraphNode
{
public:

	virtual ~AFGraphNode() = default;

	virtual void Evaluate(float deltaTime) = 0;
};

template<typename Derived>
class AFGraphNodeCRTP : public AFGraphNode
{
public:

	~AFGraphNodeCRTP() override;

	using ThisClass = Derived;

	template<typename T>
	void SetParamPropertiesTemplated(FAFParam<T> ThisClass::* ptrToMember, const FAFParam<T>& valueToApply)
	{
		ThisClass* self = static_cast<ThisClass*>(this);

		FAFParam<T>& param = self->*ptrToMember;
		param = valueToApply;
	}
};

template <typename Derived>
AFGraphNodeCRTP<Derived>::~AFGraphNodeCRTP()
{
}
