#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "AFObject.h"
#include "AFGraphNode.h"

class AFAnimGraph : public AFObject
{
public:

	void Compile(const std::string& graphString);
	void Evaluate(float deltaTime);

	void OnNodeCreated(const std::string& msg);
	void OnNodeUpdated(const std::string& msg);
	void OnNodeRemoved(const std::string& msg);

private:

	std::unordered_map<std::string, std::unique_ptr<AFGraphNode>> m_idToNode = {};
};
