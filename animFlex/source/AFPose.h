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
	void CopyPoseFrom(const AFPose& otherPose);

	const std::vector<std::shared_ptr<class AFJoint>>& GetJoints() const;

	void SetCurvesValues(const std::unordered_map<std::string, float>& curvesValues) const;
	const std::unordered_map<std::string, float>& GetCurvesValues() const;

private:

	std::vector<std::shared_ptr<class AFJoint>> m_joints = {};
	mutable std::unordered_map<std::string, float> m_curvesValues = {};

	// Creates new instances of AFJoint and copies properties onto them.
	void CreateJoints(const std::vector<std::shared_ptr<AFJoint>>& joints);
};
