#pragma once

#include <functional>
#include <memory>
#include <string>

using GraphNodeConstructorFun = std::function<std::unique_ptr<class AFGraphNode>()>;

class AFGraphNodeRegistry
{
public:

	static std::unordered_map<std::string, GraphNodeConstructorFun> GRAPH_NODE_CONSTRUCTORS;
};

template<typename T>
struct FAFGraphNodeClassRegistrar
{
	FAFGraphNodeClassRegistrar(const std::string& type)
	{
		AFGraphNodeRegistry::GRAPH_NODE_CONSTRUCTORS[type] = [type]() -> std::unique_ptr<class AFGraphNode>
			{
				return std::make_unique<T>();
			};
	}
};

#define AFCLASS(Class, ClassStringName) \
	inline static FAFGraphNodeClassRegistrar<Class> _registrar_##Class(#Class);

#define AFPARAM(Type, VarName, VarString, Direction, Meta) \
	Type VarName