#include "AFAnimationClip.h"

#include "AFGLTFLoader.h"

void AFAnimationClip::AddChannel(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim,
	tinygltf::AnimationChannel channel)
{
	std::shared_ptr<AFAnimationChannel> chan = std::make_shared<AFAnimationChannel>();
	chan->LoadChannelData(model, anim, channel);

	m_animationChannels.push_back(chan);
}

const std::vector<std::shared_ptr<AFAnimationChannel>>& AFAnimationClip::GetAnimationChannels() const
{
	return m_animationChannels;
}

float AFAnimationClip::GetClipEndTime() const
{
	return m_animationChannels.at(0)->GetMaxTime();
}

void AFAnimationClip::SetClipName(const std::string& newName)
{
	m_clipName = newName;
}

std::string AFAnimationClip::GetClipName() const
{
	return m_clipName;
}

bool AFAnimationClip::LoadImpl(const char* filepath)
{
	if (AFGLTFLoader::LoadAnim(filepath, this))
	{
		return true;
	}

	return false;
}

