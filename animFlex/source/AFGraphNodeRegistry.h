#pragma once

#include "third_party/json.hpp"
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <glm/vec3.hpp>

#include "AFPose.h"

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

template<typename T>
struct ParamTraits;

template<>
struct ParamTraits<float>
{
	static bool FromJSON(const nlohmann::json& json, float& out)
	{
		if (!json.is_string()) return false;
		std::istringstream iss(json.get<std::string>());
		return static_cast<bool>(iss >> out);
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

struct FAFParamStaticPropertyBase
{
	virtual ~FAFParamStaticPropertyBase() = default;

	virtual void Apply(std::shared_ptr<AFGraphNode> node, const nlohmann::json& valueField) const = 0;
	virtual std::string GetParamName() const = 0;
	virtual void* GetParam(std::shared_ptr<AFGraphNode> node) const = 0;
};

template<typename OwnerClassType, typename ParamType>
struct FAFParamStaticProperty : FAFParamStaticPropertyBase
{
	FAFParam<ParamType> OwnerClassType::* ptrToMember;
	std::string paramName = "";

	void Apply(std::shared_ptr<AFGraphNode> node, const nlohmann::json& valueField) const override
	{
		if (std::shared_ptr<OwnerClassType> casted = std::dynamic_pointer_cast<OwnerClassType>(node))
		{
			FAFParam<ParamType> param = {};
			bool applyValue = false;

			if (valueField.contains("value"))
			{
				const auto& val = valueField["value"];
				ParamType parsed = {};
				if (ParamTraits<ParamType>::FromJSON(val, parsed))
				{
					param.value = parsed;
					applyValue = true;
				}
			}
			else if (valueField.contains("connectedNodeId"))
			{
				param.connectedNodeId = valueField["connectedNodeId"];
				param.connectedSocketName = valueField["connectedSocketName"];
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
	inline static FAFGraphNodeClassRegistrar<Class> _registrar_##Class = FAFGraphNodeClassRegistrar<Class>(#Class); \
	std::string GetNodeType() const override {return #Class;}

#define AFPARAM(Type, VarName, VarString, Direction, Meta) \
	FAFParam<Type> VarName; \
	inline static FAFGraphNodeParamRegistrar<ThisClass, Type> _registrar_##VarName = FAFGraphNodeParamRegistrar<ThisClass, Type>(ThisClassStringName, #VarName, &ThisClass::VarName)
	