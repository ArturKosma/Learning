#pragma once

#include "third_party/json.hpp"
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <glm/vec3.hpp>
#include <type_traits>

#include "AFEvaluator.h"
#include "AFExec.h"
#include "AFPose.h"
#include "AFTimerManager.h"

class AFGraphNode;

struct FAFParamBase
{
	void SetConnection(const std::string& newConnectedNodeId, const std::string& newConnectedSocketName)
	{
		connectedNodeId = newConnectedNodeId;
		connectedSocketName = newConnectedSocketName;
	}
	bool GetConnection(std::string& outConnectedNodeId, std::string& outConnectedSocketName) const
	{
		if (connectedNodeId.empty() || connectedSocketName.empty())
		{
			return false;
		}

		outConnectedNodeId = connectedNodeId;
		outConnectedSocketName = connectedSocketName;

		return true;
	}

	std::string connectedNodeId = "";
	std::string connectedSocketName = "";
};

template<typename T>
struct FAFParam : FAFParamBase
{
public:

	FAFParam(const std::string& paramName, T defaultValue = {})
	{
		name = paramName;
		SetValue(defaultValue);
	}

	operator T()
	{
		return GetValue();
	}

	void SetValue(const T& newValue)
	{
		value = newValue;
	}
	const T& GetValue() const;

protected:

	T value = {};
	std::string name = "";
};

template<typename T>
struct ParamTraits;

template<>
struct ParamTraits<float>
{
	static bool FromJSON(const nlohmann::json& json, float& out)
	{
		if (!json.is_string()) return false;
		std::istringstream iss(json.get<std::string>());
		const bool streamApplied = static_cast<bool>(iss >> out);
		return streamApplied;
	}
};

template<>
struct ParamTraits<int>
{
	static bool FromJSON(const nlohmann::json& json, int& out)
	{
		if (!json.is_string()) return false;
		std::istringstream iss(json.get<std::string>());
		const bool streamApplied = static_cast<bool>(iss >> out);
		return streamApplied;
	}
};

template<>
struct ParamTraits<glm::vec3>
{
	static bool FromJSON(const nlohmann::json& json, glm::vec3& out)
	{
		if (json.is_array() && json.size() == 3)
		{
			out = glm::vec3(json[0], json[1], json[2]);
			return true;
		}
		return false;
	}
};

template<>
struct ParamTraits<AFPose>
{
	static bool FromJSON(const nlohmann::json& json, AFPose& out)
	{
		return false;
	}
};

template<>
struct ParamTraits<bool>
{
	static bool FromJSON(const nlohmann::json& json, bool& out)
	{
		if (!json.is_string()) return false;
		if (json.get<std::string>() == "true")
		{
			out = true;
		}
		else
		{
			out = false;
		}
		return true;
	}
};

template<>
struct ParamTraits<std::string>
{
	static bool FromJSON(const nlohmann::json& json, std::string& out)
	{
		if (!json.is_string()) return false;
		out = json.get<std::string>();
		return true;
	}
};

template<>
struct ParamTraits<AFExec>
{
	static bool FromJSON(const nlohmann::json& json, AFExec& out)
	{
		return false;
	}
};

struct FAFParamStaticPropertyBase
{
	virtual ~FAFParamStaticPropertyBase() = default;

	virtual void Apply(std::shared_ptr<AFGraphNode> node, const nlohmann::json& valueField) const = 0;
	virtual std::string GetParamName() const = 0;
	virtual void* GetParam(std::shared_ptr<AFGraphNode> node) const = 0;
	virtual bool GetIsInput() const = 0;
};

template<typename OwnerClassType, typename ParamType>
struct FAFParamStaticProperty : FAFParamStaticPropertyBase
{
	FAFParam<ParamType> OwnerClassType::* ptrToMember;
	std::string paramName = "";
	bool isInput = false;

	void Apply(std::shared_ptr<AFGraphNode> node, const nlohmann::json& valueField) const override
	{
		if (std::shared_ptr<OwnerClassType> casted = std::dynamic_pointer_cast<OwnerClassType>(node))
		{
			FAFParam<ParamType> param = FAFParam<ParamType>(paramName);
			bool applyValue = false;

			if (valueField.contains("value"))
			{
				const auto& val = valueField["value"];
				ParamType parsed = {};
				if (ParamTraits<ParamType>::FromJSON(val, parsed))
				{
					param.SetValue(parsed);
					applyValue = true;
				}
			}
			else if (valueField.contains("connectedNodeId"))
			{
				param.SetConnection(valueField["connectedNodeId"], valueField["connectedSocketName"]);
			}

			casted->template SetParamPropertiesTemplated<ParamType>(ptrToMember, param, applyValue);
		}
	}

	std::string GetParamName() const override
	{
		return paramName;
	}

	void* GetParam(std::shared_ptr<AFGraphNode> node) const override
	{
		if (std::shared_ptr<OwnerClassType> casted = std::dynamic_pointer_cast<OwnerClassType>(node))
		{
			return reinterpret_cast<void*>(casted->template GetParam<ParamType>(ptrToMember));
		}

		return nullptr;
	}

	bool GetIsInput() const override
	{
		return isInput;
	}
};

using GraphNodeConstructorFun = std::function<std::shared_ptr<class AFGraphNode>()>;

class AFGraphNodeRegistry
{
public:

	static AFGraphNodeRegistry& Get()
	{
		static AFGraphNodeRegistry singleton;
		return singleton;
	}

	void RegisterProperty(const std::string& nodeType, std::shared_ptr<FAFParamStaticPropertyBase> prop)
	{
		const auto it = GetTypeToProperties().find(nodeType);
		if (it == GetTypeToProperties().end())
		{
			GetTypeToProperties()[nodeType] = std::vector<std::shared_ptr<FAFParamStaticPropertyBase>>();
		}

		GetTypeToProperties()[nodeType].push_back(prop);
	}

	void RegisterCtor(const std::string& nodeType, GraphNodeConstructorFun ctor)
	{
		GetTypeToConstructor()[nodeType] = ctor;
	}

	std::shared_ptr<AFGraphNode> CreateNode(const std::string& nodeType, const std::string& nodeID)
	{
		const auto it = GetTypeToConstructor().find(nodeType);
		if (it == GetTypeToConstructor().end())
		{
			return nullptr;
		}

		GraphNodeConstructorFun ctor = it->second;
		std::shared_ptr<AFGraphNode> newNode = ctor();
		GetIdToNode()[nodeID] = newNode;
		GetIdToType()[nodeID] = nodeType;

		return newNode;
	}

	std::shared_ptr<AFGraphNode> GetNode(const std::string& nodeID)
	{
		const auto it = GetIdToNode().find(nodeID);
		if (it == GetIdToNode().end())
		{
			return nullptr;
		}

		return it->second;
	}

	std::string NodeIDToType(const std::string& nodeID)
	{
		const auto it = GetIdToType().find(nodeID);
		if (it == GetIdToType().end())
		{
			return "";
		}

		return it->second;
	}

	std::vector<std::shared_ptr<FAFParamStaticPropertyBase>> GetStaticProperties(const std::string& nodeType)
	{
		const auto it = GetTypeToProperties().find(nodeType);
		if (it == GetTypeToProperties().end())
		{
			return {};
		}

		return it->second;
	}

	void RemoveNode(const std::string& nodeID)
	{
		GetIdToNode().erase(nodeID);
	}

private:

	AFGraphNodeRegistry();

	std::unordered_map<std::string, GraphNodeConstructorFun>& GetTypeToConstructor()
	{
		static std::unordered_map<std::string, GraphNodeConstructorFun> map;
		return map;
	}

	std::unordered_map<std::string, std::shared_ptr<AFGraphNode>>& GetIdToNode()
	{
		static std::unordered_map<std::string, std::shared_ptr<AFGraphNode>> map;
		return map;
	}

	std::unordered_map<std::string, std::vector<std::shared_ptr<FAFParamStaticPropertyBase>>>& GetTypeToProperties()
	{
		static std::unordered_map<std::string, std::vector<std::shared_ptr<FAFParamStaticPropertyBase>>> map;
		return map;
	}

	std::unordered_map<std::string, std::string>& GetIdToType()
	{
		static std::unordered_map<std::string, std::string> map;
		return map;
	}
};

template<typename T>
struct FAFGraphNodeClassRegistrar
{
	FAFGraphNodeClassRegistrar(const std::string& type)
	{
		AFGraphNodeRegistry::Get().RegisterCtor(type, []() -> std::shared_ptr<AFGraphNode>
		{
			return std::make_shared<T>();
		});
	}
};

template<typename OwnerClassType, typename ParamType>
struct FAFGraphNodeParamRegistrar
{
	FAFGraphNodeParamRegistrar(const std::string& classStringName, const std::string& paramName, ParamType defaultValue, FAFParam<ParamType> OwnerClassType::* ptrToMember, const std::string& direction)
	{
		std::shared_ptr<FAFParamStaticProperty<OwnerClassType, ParamType>> prop = std::make_shared<FAFParamStaticProperty<OwnerClassType, ParamType>>();
		prop->ptrToMember = ptrToMember;
		prop->paramName = paramName;
		prop->isInput = direction == "Input";

		AFGraphNodeRegistry::Get().RegisterProperty(classStringName, prop);
	}
};

template <typename T>
const T& FAFParam<T>::GetValue() const
{
	// If this socket is connected to something.
	std::string connectedNodeId = "";
	std::string connectedSocketName = "";
	const bool connected = GetConnection(connectedNodeId, connectedSocketName);
	if (!connected)
	{
		// It's not connected, so just any value there currently is.
		//printf("nothing connected.\n");
		return value;
	}

	// What direction is our target?
	bool iAmInput = false;

	// Pointer to the target socket.
	std::shared_ptr<FAFParamStaticPropertyBase> targetSocket = nullptr;

	std::shared_ptr<AFGraphNode> connection = AFGraphNodeRegistry::Get().GetNode(connectedNodeId);
	const std::string& connectionNodeType = AFGraphNodeRegistry::Get().NodeIDToType(connectedNodeId);
	const std::vector<std::shared_ptr<FAFParamStaticPropertyBase>>& properties = AFGraphNodeRegistry::Get().GetStaticProperties(connectionNodeType);
	for (std::shared_ptr<FAFParamStaticPropertyBase> property : properties)
	{
		if (property->GetParamName() == connectedSocketName)
		{
			// I am the opposite of my target.
			iAmInput = !property->GetIsInput();

			// Save the pointer.
			targetSocket = property;
		}
	}

	// Mark this socket as last active.
	// Used for debug.
	if constexpr (std::is_same_v<T, AFPose>)
	{
		nlohmann::json lastActiveEntry;
		lastActiveEntry["nodeId"] = connectedNodeId;
		lastActiveEntry["socketName"] = connectedSocketName;
		AFEvaluator::Get().AddLastActiveSocket(lastActiveEntry);
	}

	if (iAmInput)
	{
		// If I am an input, we might need to ask our target node to evaluate itself.
		// If target was already evaluated we don't re-evaluate.
		// Evaluation of a node triggers its own AFParams::GetValue(), which triggers whole chain of graph evaluations.

		AFEvaluator::Get().EvaluateNode(connection);

		// Now we can fetch the value from target socket.
		return (static_cast<FAFParam<T>*>(targetSocket->GetParam(connection)))->GetValue();
	}
	else
	{
		// If I am an output I just return whatever I have currently assigned.
		return value;
	}
}

#define AFCLASS(Class, ClassStringName, Meta) \
	inline static std::string ThisClassStringName = #Class; \
	inline static FAFGraphNodeClassRegistrar<Class> _registrar_##Class = FAFGraphNodeClassRegistrar<Class>(#Class); \
	std::string GetNodeType() const override {return #Class;}

#define AFPARAM(Type, VarName, DefaultValue, VarString, Direction, Meta) \
	FAFParam<Type> VarName = FAFParam<Type>(#VarName, DefaultValue); \
	inline static FAFGraphNodeParamRegistrar<ThisClass, Type> _registrar_##VarName = FAFGraphNodeParamRegistrar<ThisClass, Type>(ThisClassStringName, #VarName, DefaultValue, &ThisClass::VarName, Direction)

#define AFENUM() 
	