#pragma once
#include <string>
#include <vector>

#include "AFAsset.h"

class AFFloatCurve : public AFAsset
{

public:

	// Takes in a value and tries to return time for that value.
	float SampleByValue(float value);

protected:

	bool LoadImpl(const char* filepath) override;

	std::vector<float> timings = {};
	std::vector<float> values = {};
};