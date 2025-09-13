#pragma once
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AFPose.h"
#include "AFJoint.h"
#include "AFUtility.h"

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

	static glm::mat4 IdentityTransform()
	{
		return glm::mat4(1.0f);
	}

	static glm::quat Vec4ToQuat(const glm::vec4& vec4)
	{
		return glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);
	}

	static glm::vec3 RotationFromDirection(const glm::vec3& dir)
	{
		const float yaw = -glm::degrees(atan2(dir.x, dir.z)); // Yaw around Y.
		const float pitch = glm::degrees(asin(dir.y)); // Pitch up/down.

		return glm::vec3(pitch, yaw, 0.0f);
	}

	static glm::vec3 DirectionFromRotation(const glm::vec3& rot)
	{
		const float pitch = glm::radians(rot.x);
		const float yaw = -glm::radians(rot.y);

		const float x = sin(yaw) * cos(pitch);
		const float y = sin(pitch);
		const float z = cos(yaw) * cos(pitch);

		return glm::normalize(glm::vec3(x, y, z));
	}

	static glm::quat QuaternionFromDirection(const glm::vec3 direction, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		glm::vec3 forward = glm::normalize(direction);
		glm::vec3 right = glm::normalize(glm::cross(up, forward));
		glm::vec3 newUp = glm::cross(forward, right);

		glm::mat3 rotMat(right, newUp, forward);
		return glm::quat_cast(rotMat);
	}

	static glm::quat QuaternionFromEuler(const glm::vec3 euler)
	{
		return glm::quat(glm::radians(euler));
	}

	static glm::vec3 EulerFromQuaternion(const glm::quat quat)
	{
		return glm::vec3(glm::degrees(glm::eulerAngles(quat)));
	}

	static glm::quat QInterpTo(const glm::quat& current, const glm::quat& target, float interpSpeed, float deltaTime)
	{
		if (interpSpeed <= 0.0f)
		{
			return target;
		}

		glm::quat currentNorm = glm::normalize(current);
		glm::quat targetNorm = glm::normalize(target);

		// Ensure shortest path.
		if (glm::dot(currentNorm, targetNorm) < 0.0f)
		{
			targetNorm = -targetNorm;
		}

		float dot = glm::dot(currentNorm, targetNorm);
		dot = glm::clamp(dot, -1.0f, 1.0f);
		const float angle = 2.0f * std::acos(dot);

		if (angle < 1e-6f)
		{
			return targetNorm;
		}

		const float maxStep = interpSpeed * deltaTime;
		const float t = glm::clamp(maxStep / angle, 0.0f, 1.0f);

		return glm::slerp(currentNorm, targetNorm, t);
	}

	static glm::vec3 VInterpTo(const glm::vec3& current, const glm::vec3& target, float interpSpeed, float deltaTime)
	{
		if (interpSpeed <= 0.0f)
		{
			return target;
		}

		glm::vec3 delta = target - current;
		float dist = glm::length(delta);
		if (dist < 1e-6f)
		{
			return target;
		}

		float maxStep = interpSpeed * deltaTime;
		if (dist <= maxStep)
		{
			return target;
		}

		return current + (delta / dist) * maxStep;
	}

	static float FInterpToConst(float current, float target, float interpSpeed, float deltaTime)
	{
		if (NearlyEqual(interpSpeed, 0.0f))
		{
			return target;
		}

		const float dist = glm::abs(target - current);
		const float step = std::max(interpSpeed, 0.0f) * std::max(deltaTime, 0.0f);
		const float dir = glm::sign(target - current);

		if (step <= 0.0f || dist == 0.0f)
		{
			return current;
		}

		if (dist <= step)
		{
			return target;
		}

		return current + (step * dir);
	}

	static float FInterpToTime(float current, float target, float interpSpeed, float deltaTime)
	{
		if (NearlyEqual(interpSpeed, 0.0f))
		{
			return target;
		}

		const float dist = glm::abs(target - current);
		const float step = std::max(interpSpeed, 0.0f) * std::max(deltaTime, 0.0f);
		const float dir = glm::sign(target - current);

		if (step <= 0.0f || dist == 0.0f)
		{
			return current;
		}

		if (dist <= step)
		{
			return target;
		}

		return current + (step * dir);
	}

	static float DeltaAngle(float current, float target)
	{
		float diff = fmodf(target - current + 180.0f, 360.0f);

		if (diff < 0.0f)
		{
			diff += 360.0f;
		}

		return diff - 180.0f;
	}

	static float NormalizeAngle(float angle)
	{
		float result = fmodf(angle + 180.0f, 360.0f);
		if (result < 0.0f)
		{
			result += 360.0f;
		}

		return result - 180.0f;
	}

	static float FInterpToAngle(float currentAngle, float targetAngle, float interpSpeed, float deltaTime)
	{
		const float delta = DeltaAngle(currentAngle, targetAngle);
		const float step = interpSpeed * deltaTime;
		if (std::fabs(delta) <= step)
		{
			return delta;
		}

		return delta > 0.0f ? step : -step;
	}

	static bool NearlyEqual(float a, float b, float threshold = glm::epsilon<float>())
	{
		return glm::epsilonEqual(a, b, threshold);
	}

	static void BlendPoses(const AFPose& outPose, const AFPose& a, const AFPose& b, float alpha)
	{
		for (size_t i = 0; i < a.GetJoints().size(); ++i)
		{
			glm::vec3 blendLoc = glm::mix(a.GetJoints()[i]->GetLocation(), b.GetJoints()[i]->GetLocation(), alpha);
			glm::quat blendRot = glm::slerp(a.GetJoints()[i]->GetRotation(), b.GetJoints()[i]->GetRotation(), alpha);
			glm::vec3 blendScale = glm::mix(a.GetJoints()[i]->GetScale(), b.GetJoints()[i]->GetScale(), alpha);

			outPose.GetJoints()[i]->SetLocation(blendLoc);
			outPose.GetJoints()[i]->SetRotation(blendRot);
			outPose.GetJoints()[i]->SetScale(blendScale);

			outPose.GetJoints()[i]->CalculateLocalTRSMatrix();
		}

		// Blend curves.
		std::unordered_map<std::string, float> blended;
		BlendCurveValues(blended, a.GetCurvesValues(), b.GetCurvesValues(), alpha);

		outPose.SetCurvesValues(blended);
	}

	static void BlendCurveValues(std::unordered_map<std::string, float>& out, std::unordered_map<std::string, float> a, std::unordered_map<std::string, float> b, float alpha)
	{
		// Blend curves.
		for (const std::string& name : AFUtility::GetCurveNames()) 
		{
			out[name] = glm::mix(a[name], b[name], alpha);
		}
	}

	template<typename T>
	static size_t NearestIndex(const std::vector<T>& array, const T& value)
	{
		if (array.empty())
		{
			return 0;
		}

		// High index.
		size_t hi = std::lower_bound(array.begin(), array.end(), value) - array.begin();

		// Clamps.
		if (hi <= 0)
		{
			return 0;
		}
		else if (hi >= array.size())
		{
			return array.size() - 1;
		}

		// Low index.
		size_t lo = std::max(static_cast<size_t>(0), hi - 1);

		// Choose closer.
		const T distHi = std::abs(value - array[hi]);
		const T distLo = std::abs(value - array[lo]);

		return (distHi < distLo) ? hi : lo;
	}

	static float SignedAngleBetweenVectors(const glm::vec3& vec1, const glm::vec3& vec2, const glm::vec3& up)
	{
		const float dot = glm::dot(vec1, vec2);
		//printf("%f\n", dot);
		const float crossSign = glm::dot(glm::cross(vec1, vec2), glm::normalize(up));

		const float rad = glm::atan(crossSign, dot);

		return glm::degrees(rad);
	}

	static void QuaternionTwist(const glm::quat& quat, const glm::vec3& axis, glm::quat& outQuat, float& outAngle)
	{
		const glm::vec3 axisNorm = glm::normalize(axis);
		const glm::vec3 real = glm::vec3(quat.x, quat.y, quat.z);
		const float proj = glm::dot(real, axisNorm);

		const glm::vec3 twistVec = proj * axisNorm;

		const glm::quat twistQuat = glm::quat(quat.w, twistVec.x, twistVec.y, twistVec.z);
		const float angle = 2.0f * atan2(glm::length(glm::vec3(twistQuat.x, twistQuat.y, twistQuat.z)), twistQuat.w);
		const float sign = (proj >= 0.0f) ? 1.0f : -1.0f;

		outAngle = glm::degrees(angle) * sign;
		outQuat = glm::angleAxis(glm::radians(outAngle), axis);
	}
};
