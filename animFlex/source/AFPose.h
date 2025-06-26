#pragma once
#include "AFAnimationClip.h"

class AFPose
{
public:

	// #hack
	// AFPose gets naively initialized to the mannequin skeleton in binding pose.
	// Usually this should initialize to the provided skeleton.
	AFPose();

	// Creates new instances of AFJoint and copies properties onto them.
	void ApplyJoints(const std::vector<std::shared_ptr<AFJoint>>& joints);
	void ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time);

	const std::vector<std::shared_ptr<class AFJoint>>& GetJoints() const;

private:

	std::vector<std::shared_ptr<class AFJoint>> m_joints;
};
