#pragma once

#include "third_party/json.hpp"
#include <functional>
#include <memory>
#include <sstream>
#include <string>

class AFGraphNode;

template<typename T>
struct FAFParam
{
public:

	operator T()
	{
		return value;
	}

	T value = {};
	std::string connectedNodeId = "";
	std::string connectedSocketName = "";
};

struct FAFParamStaticPropertyBase
{
	virtual ~FAFParamStaticPropertyBase() = default;

	virtual void Apply(std::shared_ptr<AFGraphNode> node, const std::string& msg) const = 0;
	virtual std::string GetParamName() const = 0;
};

template<typename OwnerClassType, typename ParamType>
struct FAFParamStaticProperty : FAFParamStaticPropertyBase
{
	FAFParam<ParamType> OwnerClassType::* ptrToMember;
	std::string paramName = "";

	void Apply(std::shared_ptr<AFGraphNode> node, const std::string& msg) const override
	{
		if (std::shared_ptr<OwnerClassType> casted = std::dynamic_pointer_cast<OwnerClassType>(node))
		{
			// Create actual values to assign to the param described by the msg.
			// ptrToMember allows us to pinpoint the exact place in memory where to apply the FAFParam<ParamType>.
			FAFParam<ParamType> valueToApply = CreateParamFromMsg(msg);

			casted->template SetParamPropertiesTemplated<ParamType>(ptrToMember, valueToApply);
		}
	}

	std::string GetParamName() const override
	{
		return paramName;
	}

	FAFParam<ParamType> CreateParamFromMsg(const std::string& msg) const
	{
		FAFParam<ParamType> ret = {};

		// Find the position of the first colon.
		const size_t colonPos = msg.find(':');

		// Extract the substring from the start up to the colon.
		const std::string& edit = msg.substr(0, colonPos);

		if (edit == "value")
		{
			const std::string& valueStr = msg.substr(colonPos + 1);

			std::istringstream iss(valueStr);
			ParamType value;
			if (iss >> value) 
			{
				ret.value = value;
			}
		}
		else if(edit == "connection")
		{
			const std::string& connectionStr = msg.substr(colonPos + 1);
			const size_t slashPos = connectionStr.find('/');

			const std::string& connectionNodeId = connectionStr.substr(0, slashPos);
			const std::string& connectionSocketName = connectionStr.substr(slashPos + 1);

			ret.connectedNodeId = connectionNodeId;
			ret.connectedSocketName = connectionSocketName;
		}

		return ret;
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
	FAFGraphNodeParamRegistrar(const std::string& classStringName, const std::string& paramName, FAFParam<ParamType> OwnerClassType::* ptrToMember)
	{
		std::shared_ptr<FAFParamStaticProperty<OwnerClassType, ParamType>> prop = std::make_shared<FAFParamStaticProperty<OwnerClassType, ParamType>>();
		prop->ptrToMember = ptrToMember;
		prop->paramName = paramName;

		AFGraphNodeRegistry::Get().RegisterProperty(classStringName, prop);
	}
};

#define AFCLASS(Class, ClassStringName, Meta) \
	inline static std::string ThisClassStringName = #Class; \
	inline static FAFGraphNodeClassRegistrar<Class> _registrar_##Class = FAFGraphNodeClassRegistrar<Class>(#Class)

#define AFPARAM(Type, VarName, VarString, Direction, Meta) \
	FAFParam<Type> VarName; \
	inline static FAFGraphNodeParamRegistrar<ThisClass, Type> _registrar_##VarName = FAFGraphNodeParamRegistrar<ThisClass, Type>(ThisClassStringName, #VarName, &ThisClass::VarName)
	