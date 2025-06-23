#pragma once
#include "AFAnimationClip.h"

class AFPose
{
public:

	void ApplyJoints(const std::vector<std::shared_ptr<AFJoint>>& joints);
	void ApplyClip(std::shared_ptr<AFAnimationClip> clip, float time);

	const std::vector<std::shared_ptr<class AFJoint>>& GetJoints() const;

private:

	std::vector<std::shared_ptr<class AFJoint>> m_joints;
};
