#pragma once
#include <memory>
#include <string>

#include "AFAnimationChannel.h"
#include "AFStructs.h"
#include "third_party/tiny_gltf.h"

class AFAnimationClip : public FAFAsset
{
public:

	void AddChannel(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim, tinygltf::AnimationChannel channel);
	const std::vector<std::shared_ptr<AFAnimationChannel>>& GetAnimationChannels() const;

	float GetClipEndTime() const;

	void SetClipName(const std::string& newName);
	std::string GetClipName() const;

protected:

	bool LoadImpl(const char* filepath) override;

private:

	std::vector<std::shared_ptr<AFAnimationChannel>> m_animationChannels = {};
	std::string m_clipName = {};
};
