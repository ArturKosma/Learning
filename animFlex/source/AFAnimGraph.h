#pragma once
#include <string>
#include "AFObject.h"
#include "AFPose.h"
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

	const AFPose& GetFinalPose() const;

	static void AddLastActiveSocket(const nlohmann::json& newSocket);
	static const std::string& GetLastActiveSockets();
	static void ClearLastActiveSockets();

protected:

	std::shared_ptr<AFGraphNode_OutputPose> m_outputPoseNode = nullptr;
	AFPose m_finalPose = {};

	static nlohmann::json lastActiveSockets;
};
