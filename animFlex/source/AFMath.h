#pragma once
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

		// Handle reversed output range.
		if (outMin < outMax)
		{
			return glm::clamp(mappedValue, outMin, outMax);
		}
		else
		{
			return glm::clamp(mappedValue, outMax, outMin);
		}
	}

	static glm::mat4 TLerp(const glm::mat4& a, const glm::mat4& b, float alpha)
	{
		glm::vec3 skewA;
		glm::vec4 perspectiveA;
		glm::vec3 scaleA;
		glm::quat rotationA;
		glm::vec3 translationA;

		glm::vec3 skewB;
		glm::vec4 perspectiveB;
		glm::vec3 scaleB;
		glm::quat rotationB;
		glm::vec3 translationB;

		glm::decompose(a, scaleA, rotationA, translationA, skewA, perspectiveA);
		glm::decompose(b, scaleB, rotationB, translationB, skewB, perspectiveB);

		glm::vec3 interpPos = glm::mix(translationA, translationB, alpha);
		glm::vec3 interpScale = glm::mix(scaleA, scaleB, alpha);
		glm::quat interpRot = glm::slerp(rotationA, rotationB, alpha);

		glm::mat4 transMat = glm::translate(glm::mat4(1.0f), interpPos);
		glm::mat4 rotMat = glm::toMat4(interpRot);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), interpScale);

		return transMat * rotMat * scaleMat;
	}

	static float AngleLerp(float a, float b, float alpha)
	{
		const float diff = glm::mod(b - a + 180.0f, 360.0f) - 180.0f;
		return a + diff * alpha;
	}

	static glm::mat4 ComposeTransform(const glm::vec3& translation, const glm::quat& rotation, const glm::vec3& scale)
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), translation);
		glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

		return translationMatrix * rotationMatrix * scaleMatrix;
	}

	static glm::quat Vec4ToQuat(const glm::vec4& vec4)
	{
		return glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);
	}
};
