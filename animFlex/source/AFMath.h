#pragma once
#include <glm/glm.hpp>

class AFMath
{
public:

	static glm::vec3 GetSafeNormal(glm::vec3 vec3)
	{
		return glm::length(vec3) > 0.0f ? glm::normalize(vec3) : glm::vec3(0.0f);
	}

	static float MapRangeClamped(float value, float inMin, float inMax, float outMin, float outMax)
	{
		if (inMax - inMin == 0.0f) 
		{
			return outMin;
		}

		const float mappedValue = (value - inMin) / (inMax - inMin) * (outMax - outMin) + outMin;

		return glm::clamp(mappedValue, outMin, outMax);
	}
};
