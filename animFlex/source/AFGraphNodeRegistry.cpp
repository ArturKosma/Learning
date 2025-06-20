#include "AFGraphNodeRegistry.h"

std::unordered_map<std::string, GraphNodeConstructorFun> AFGraphNodeRegistry::GRAPH_NODE_CONSTRUCTORS = {};
