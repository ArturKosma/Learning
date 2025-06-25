#pragma once

#include "AFGraphNodeRegistry.h"

class AFGraphNode
{
public:

	virtual ~AFGraphNode() = default;

	virtual void Init();
	virtual void OnUpdate();
	virtual void Evaluate(float deltaTime) = 0;

	virtual std::string GetNodeType() const = 0;

	bool operator==(const AFGraphNode& other) const
	{
		return GetNodeType() == other.GetNodeType();
	}
};

template<typename Derived>
class AFGraphNodeCRTP : public AFGraphNode
{
public:

	~AFGraphNodeCRTP() override;

	using ThisClass = Derived;

	template<typename T>
	void SetParamPropertiesTemplated(FAFParam<T> ThisClass::* ptrToMember, const FAFParam<T>& paramProps, bool applyValue)
	{
		ThisClass* self = static_cast<ThisClass*>(this);

		FAFParam<T>& param = self->*ptrToMember;

		if (applyValue)
		{
			param.SetValue(paramProps.GetValue());
		}

		std::string connectedNodeId = "";
		std::string connectedSocketName = "";
		paramProps.GetConnection(connectedNodeId, connectedSocketName);
		param.SetConnection(connectedNodeId, connectedSocketName);
	}

	template<typename T>
	FAFParam<T>* GetParam(FAFParam<T> ThisClass::* ptrToMember)
	{
		ThisClass* self = static_cast<ThisClass*>(this);
		return &(self->*ptrToMember);
	}
};

template <typename Derived>
AFGraphNodeCRTP<Derived>::~AFGraphNodeCRTP()
{
}
