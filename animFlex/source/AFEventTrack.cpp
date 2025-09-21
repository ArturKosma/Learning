#include "AFEventTrack.h"
#include <fstream>
#include "AFAnimationClip.h"
#include "AFContent.h"
#include "third_party/json.hpp"

void AFEventTrack::OnLoadComplete()
{
	const std::string& eventTrackName = GetName();

	std::vector<std::shared_ptr<AFAnimationClip>> anims = AFContent::Get().FindAllAssetsOfType<AFAnimationClip>();
	for (const std::shared_ptr<AFAnimationClip>& anim : anims)
	{
		const std::string& animName = anim->GetName();
		const size_t idx = eventTrackName.find(animName);
		if (idx != std::string::npos)
		{
			anim->AddEventTrack(AFContent::Get().FindAsset<AFEventTrack>(eventTrackName.c_str()));
			return;
		}
	}
}

std::vector<std::string> AFEventTrack::SampleByTime(float start, float end)
{
	if (end < start)
	{
		return {};
	}

	auto it_start = std::lower_bound(m_timings.begin(), m_timings.end(), start);
	auto it_end = std::upper_bound(m_timings.begin(), m_timings.end(), end);

	// If start is beyond the last timing, nothing to return.
	if (it_start == m_timings.end())
	{
		return {};
	}

	size_t start_idx = static_cast<size_t>(it_start - m_timings.begin());
	size_t end_idx = static_cast<size_t>(it_end - m_timings.begin());

	// Clamp end to available events.
	end_idx = std::min(end_idx, m_events.size());

	std::vector<std::string> ret;
	for (size_t i = start_idx; i < end_idx; ++i)
	{
		const auto& e = m_events[i];
		ret.insert(ret.end(), e.begin(), e.end());
	}

	return ret;
}

float AFEventTrack::GetEventTiming(float timing, EAFEventTiming timingMode, std::vector<std::string> search,
	std::string& foundEvent) const
{
	if (search.empty())
	{
		return {};
	}

	if (timing < 0.0f)
	{
		return -1.0f;
	}

	size_t marker = timingMode == EAFEventTiming::Before ?
		std::lower_bound(m_timings.begin(), m_timings.end(), timing) - m_timings.begin() :
		std::upper_bound(m_timings.begin(), m_timings.end(), timing) - m_timings.begin();
	if (timingMode == EAFEventTiming::Before)
	{
		for (size_t i = marker; i-- > 0; )
		{
			for (const std::string& event : m_events[i])
			{
				if (std::find(search.begin(), search.end(), event) != search.end())
				{
					foundEvent = event;
					return m_timings[i];
				}
			}
		}
	}
	else
	{
		for (size_t i = marker; i < m_timings.size(); ++i)
		{
			for (const std::string& event : m_events[i])
			{
				if (std::find(search.begin(), search.end(), event) != search.end())
				{
					foundEvent = event;
					return m_timings[i];
				}
			}
		}
	}

	return -1.0f;
}

std::vector<float> AFEventTrack::GetEventTimingsOrdered(std::vector<std::string> search)
{
	if (search.empty())
	{
		return {};
	}

	std::vector<float> ret = {};

	size_t marker = 0;
	for (size_t i = 0; i < m_events.size(); ++i)
	{
		const std::vector<std::string>& events = m_events[i];
		for (const std::string& event : events)
		{
			if (event == search[marker])
			{
				ret.push_back(m_timings[i]);
				marker++;

				if (marker == search.size())
				{
					return ret;
				}
			}
		}
	}

	return marker == search.size() ? ret : std::vector<float>();
}

bool AFEventTrack::LoadImpl(const char* filepath)
{
	std::ifstream f(filepath);
	nlohmann::json jsonEv = nlohmann::json::parse(f);
	if (!jsonEv.is_array())
	{
		return false;
	}

	// Read the timings & events from json.
	for (auto& element : jsonEv)
	{
		if (!element.is_array() || element.size() < 2)
		{
			return false;
		}

		if (!element[0].is_number_float() || !element[1].is_array())
		{
			return false;
		}

		const float timing = element[0].get<float>();
		const std::vector<std::string>& events = element[1].get<std::vector<std::string>>();

		m_timings.push_back(timing);
		m_events.push_back(events);
	}

	return true;
}
