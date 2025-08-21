#pragma once
#include <memory>
#include <string>

#include "AFAnimationChannel.h"
#include "AFAsset.h"
#include "AFFloatCurve.h"
#include "AFStructs.h"
#include "third_party/tiny_gltf.h"

class AFAnimationClip : public AFAsset
{
public:
	void OnLoadComplete() override;

	void AddChannel(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim, tinygltf::AnimationChannel channel);
	const std::vector<std::shared_ptr<AFAnimationChannel>>& GetAnimationChannels() const;

	float GetClipEndTime() const;

	void SetClipName(const std::string& newName);
	std::string GetClipName() const;

	void AddCurve(const std::string& curveName, std::shared_ptr<AFFloatCurve> newCurve);
	std::shared_ptr<AFFloatCurve> GetCurve(const std::string& curveName);
	const std::unordered_map<std::string, std::shared_ptr<AFFloatCurve>>& GetCurves() const;

protected:

	bool LoadImpl(const char* filepath) override;
	bool DeserializeImpl(const char* stream, size_t len) override;

private:

	std::vector<std::shared_ptr<AFAnimationChannel>> m_animationChannels = {};
	std::string m_clipName = {};
	std::unordered_map<std::string, std::shared_ptr<AFFloatCurve>> m_curves = {};
};
