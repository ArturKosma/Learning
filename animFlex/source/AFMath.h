#pragma once
#include <glm/glm.hpp>

class AFMath
{
public:

	static glm::vec3 GetSafeNormal(glm::vec3 inVec3)
	{
		return glm::length(inVec3) > 0.0f ? glm::normalize(inVec3) : glm::vec3(0.0f);
	}
};
