#include "AFAnimationClip.h"

#include "AFGLTFLoader.h"

void AFAnimationClip::AddChannel(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim,
	tinygltf::AnimationChannel channel)
{
	std::shared_ptr<AFAnimationChannel> chan = std::make_shared<AFAnimationChannel>();
	chan->LoadChannelData(model, anim, channel);

	m_animationChannels.push_back(chan);
}

void AFAnimationClip::SetAnimationFrame(const std::vector<std::shared_ptr<AFNode>>& nodes, float time)
{
	for(auto& channel : m_animationChannels)
	{
		int targetNode = channel->GetTargetNode() - 1; // #hack The -1 is due to some problems that I have with construction of the skeleton tree. Temporary hack.

		switch(channel->GetTargetPath())
		{
			case EAFTargetPath::Translation:
			{
				nodes.at(targetNode)->SetLocation(channel->GetTranslation(time));
				break;
			}
			case EAFTargetPath::Rotation:
			{
				nodes.at(targetNode)->SetRotation(channel->GetRotation(time));
				break;
			}
			case EAFTargetPath::Scale:
			{
				nodes.at(targetNode)->SetScale(channel->GetScale(time));
				break;
			}
			default:
			{
				break;
			}
		}
	}

	for(auto& node : nodes)
	{
		node->CalculateLocalTRSMatrix();
	}
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
	if (AFGLTFLoader::LoadAnim(filepath, this, false))
	{
		return true;
	}

	return false;
}

