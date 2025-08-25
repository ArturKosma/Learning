#pragma once
#include <string>
#include <vector>

#include "AFAsset.h"

class AFFloatCurve : public AFAsset
{
public:

	// Called when curve is added to the content.
	// Should be unique.
	void OnLoadComplete() override;

	// Takes in a value and tries to return time for that value.
	float SampleByValue(float value);

	// Takes in a time and tries to return value for that time.
	float SampleByTime(float time);

protected:

	bool LoadImpl(const char* filepath) override; // Added locally.
	bool DeserializeImpl(const char* stream, size_t len) override; // Added via download.

	std::vector<float> timings = {};
	std::vector<float> values = {};
};