#include "AFFloatCurve.h"
#include <fstream>

#include "AFContent.h"
#include "AFMath.h"
#include "third_party/json.hpp"

void AFFloatCurve::OnLoadComplete()
{
	const std::string& curveName = GetName();
	//printf("loaded curve: %s\n", curveName.c_str());

	std::vector<std::shared_ptr<AFAnimationClip>> anims = AFContent::Get().FindAllAssetsOfType<AFAnimationClip>();
	for (const std::shared_ptr<AFAnimationClip>& anim : anims)
	{
		const std::string& animName = anim->GetName();
		const size_t idx = curveName.find(animName);
		if (idx != std::string::npos)
		{
			const size_t len = animName.size();
			const std::string& curveLastName = curveName.substr(len + 1, curveName.size());

			//printf("FOUND ANIM: %s\n", animName.c_str());

			anim->AddCurve(curveLastName, AFContent::Get().FindAsset<AFFloatCurve>(curveName.c_str()));

			return;
		}
	}
}

float AFFloatCurve::SampleByValue(float value)
{
	// High index.
	size_t hi = std::lower_bound(values.begin(), values.end(), value) - values.begin();

	// Low index.
	size_t lo = std::max(static_cast<size_t>(0), hi - 1);

	// Clamps.
	if (hi <= 0)
	{
		return timings.front();
	}
	if (hi >= values.size())
	{
		return timings.back();
	}

	// Fetches.
	const float v_lo = values[lo];
	const float v_hi = values[hi];
	const float t_lo = timings[lo];
	const float t_hi = timings[hi];

	// Flat segment.
	if (AFMath::NearlyEqual(v_lo, v_hi))
	{
		return (t_lo + t_hi) * 0.5f;
	}

	// Find normalized value between v_lo and v_hi.
	const float normVal = (value - v_lo) / (v_hi - v_lo);

	// Use that normalized value to lerp between two time points t_lo and t_hi.
	return t_lo + normVal * (t_hi - t_lo);
}

float AFFloatCurve::SampleByTime(float time)
{
	// High index.
	size_t hi = std::lower_bound(timings.begin(), timings.end(), time) - timings.begin();

	// Low index.
	size_t lo = std::max(static_cast<size_t>(0), hi - 1);

	// Clamps.
	if (hi <= 0)
	{
		return values.front();
	}
	if (hi >= values.size())
	{
		return values.back();
	}

	// Fetches.
	const float v_lo = values[lo];
	const float v_hi = values[hi];
	const float t_lo = timings[lo];
	const float t_hi = timings[hi];

	// Find normalized time between t_lo and t_hi.
	const float normTime = (time - t_lo) / (t_hi - t_lo);

	// Use that normalized time to lerp between two value points v_lo and v_hi.
	return v_lo + normTime * (v_hi - v_lo);
}

bool AFFloatCurve::LoadImpl(const char* filepath)
{
	std::ifstream f(filepath);
	nlohmann::json jsonCrv = nlohmann::json::parse(f);
	if (!jsonCrv.is_array())
	{
		return false;
	}

	// Read the timings & values from json.
	for (auto& element : jsonCrv)
	{
		const float timing = element[0];
		const float value = element[1];

		timings.push_back(timing);
		values.push_back(value);
	}

	return true;
}

bool AFFloatCurve::DeserializeImpl(const char* stream, size_t len)
{
	nlohmann::json jsonCrv = nlohmann::json::parse(stream, stream + len, nullptr, false);
	if (!jsonCrv.is_array())
	{
		return false;
	}

	// Read the timings & values from json.
	for (auto& element : jsonCrv)
	{
		const float timing = element[0];
		const float value = element[1];

		timings.push_back(timing);
		values.push_back(value);
	}

	return true;
}
