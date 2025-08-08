#pragma once
#include "AFAnimationClip.h"

class AFPose
{
public:

	// #hack
	// AFPose gets initialized to the mannequin skeleton in binding pose.
	// Usually this should initialize to the provided skeleton.
	AFPose();

	AFPose(const AFPose& otherPose);
	AFPose& operator=(const AFPose& otherPose);

	void ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time, bool forceRootLock = false);
	void CopyTransformsFrom(const AFPose& otherPose);

	const std::vector<std::shared_ptr<class AFJoint>>& GetJoints() const;

private:

	std::vector<std::shared_ptr<class AFJoint>> m_joints;

	// Creates new instances of AFJoint and copies properties onto them.
	void CreateJoints(const std::vector<std::shared_ptr<AFJoint>>& joints);
};
