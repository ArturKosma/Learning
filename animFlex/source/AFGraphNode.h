#pragma once

#include "AFGraphNodeRegistry.h"

class AFGraphNode
{
	friend class AFAnimGraph;
	friend class AFStateMachine;
	friend class AFAnimGraphCond;

public:

	template<typename ...Types>
	static bool IsAnyOf(std::shared_ptr<AFGraphNode> node)
	{
		return static_cast<bool>((... || std::dynamic_pointer_cast<Types>(node)));
	}

	virtual ~AFGraphNode() = default;

	virtual void Init();
	virtual void OnUpdate();
	virtual void Evaluate(float deltaTime) = 0;
	virtual void OnReset();

	std::string GetNodeID() const;
	virtual std::string GetNodeType() const = 0;

	bool operator==(const AFGraphNode& other) const
	{
		return GetNodeType() == other.GetNodeType();
	}

	void AddSubNode(const std::string& subNodeId);
	std::vector<std::string> GetSubNodes() const;

	void SetNodeContext(const std::string& contextID);
	const std::string& GetNodeContext() const;

protected:

	std::string m_nodeId = "";
	std::string m_nodeContext = "";
	std::vector<std::string> m_subNodes = {};
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
