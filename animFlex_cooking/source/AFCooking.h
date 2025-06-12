#pragma once

#include <string>
#include <glm.hpp>

enum class EAFTargetPath : uint8_t
{
	Rotation,
	Translation,
	Scale
};

enum class EAFInterpolationType : uint8_t
{
	Step,
	Linear,
	CubicSpline,
	CubicHermite
};

struct FAFAnimationChannelData
{
	uint16_t targetJoint = 0;
	EAFTargetPath targetPath = EAFTargetPath::Rotation;
	EAFInterpolationType interpType = EAFInterpolationType::Linear;
	uint32_t keyCount = 0;

	float* timings = nullptr;
	glm::vec4* values = nullptr;
};

class AFCooking
{
public:

	static std::string CookFile(const std::string& type, const std::string& sourcePath, const std::string& targetPath);

private:

	static std::string CookAnim(const std::string& sourcePath, const std::string& targetPath);
};
