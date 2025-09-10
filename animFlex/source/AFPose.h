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

	std::shared_ptr<class AFJoint> GetJoint(const std::string& jointName) const;

	void RecalculateSkeleton();

	// This is the same structure as AFMesh has.
	// We need local per-pose to be able to recalculate chain within a pose.
	// @todo Do I have to keep similar structure in AFPoses? Or can it somehow be shared with AFMesh?
	std::vector<int> m_nodeToJoint = {};
	std::vector<glm::mat4> m_inverseBindMatrices = {};
	std::vector<glm::mat4> m_jointMatrices = {};
	std::vector<glm::mat4> m_jointDualQuats = {};

private:

	std::vector<std::shared_ptr<class AFJoint>> m_joints = {};
	mutable std::unordered_map<std::string, float> m_curvesValues = {};
	std::unordered_map<std::string, size_t> m_boneNameToIndex = {};

	// Creates new instances of AFJoint and copies properties onto them.
	void CreateJoints(const std::vector<std::shared_ptr<AFJoint>>& joints);
};
