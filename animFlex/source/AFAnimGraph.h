#pragma once
#include <string>
#include "AFObject.h"
#include "AFPose.h"
#include "third_party/json.hpp"

class AFGraphNode_OutputPose;

class AFAnimGraph : public AFObject
{
public:

	virtual void Evaluate(float deltaTime);

	virtual void OnNodeCreated(const std::string& msg);
	static void OnNodeUpdated(const std::string& msg);
	static void OnNodeRemoved(const std::string& msg);

	const AFPose& GetFinalPose() const;

protected:

	AFPose m_finalPose = {};

private:

	std::shared_ptr<AFGraphNode_OutputPose> m_outputPoseNode = nullptr;
};
