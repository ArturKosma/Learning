#pragma once

#include <vector>
#include "AFAsset.h"

enum class EAFEventTiming;

class AFEventTrack : public AFAsset
{
public:

	// Called when event track is added to the content.
	void OnLoadComplete() override;

	// Takes in a time frame and returns events that happen in that time period.
	std::vector<std::string> SampleByTime(float start, float end);

	// Finds any of given events before or after specified time.
	float GetEventTiming(float timing, EAFEventTiming timingMode, std::vector<std::string> search, std::string& foundEvent) const;

	// Finds timings of specified events in order.
	std::vector<float> GetEventTimingsOrdered(std::vector<std::string> search);

protected:

	bool LoadImpl(const char* filepath) override; // Load locally (from disc).

	std::vector<float> m_timings = {};
	std::vector<std::vector<std::string>> m_events = {};
};
