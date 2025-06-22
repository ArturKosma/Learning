#pragma once
#include <string>
#include "AFObject.h"
#include "third_party/json.hpp"

class AFGraphNode_OutputPose;

class AFAnimGraph : public AFObject
{
public:

	void Compile(const std::string& graphString);
	void Evaluate(float deltaTime);

	void OnNodeCreated(const std::string& msg);
	void OnNodeUpdated(const std::string& msg);
	void OnNodeRemoved(const std::string& msg);

	std::string ReadJSONSocketValue(const nlohmann::json& socket);

protected:

	std::shared_ptr<AFGraphNode_OutputPose> m_outputPoseNode = nullptr;
};
