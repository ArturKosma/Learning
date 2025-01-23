#pragma once

struct AFRotator
{
	AFRotator()
	{

	}

	AFRotator(const glm::vec3& eulerAngles)
	{
		pitch = eulerAngles.x;
		yaw = eulerAngles.y;
		roll = eulerAngles.z;
	}

	AFRotator(float newPitch, float newYaw, float newRoll)
	{
		pitch = newPitch;
		yaw = newYaw;
		roll = newRoll;
	}

	glm::quat Quat() const
	{
		return glm::quat(glm::vec3(pitch, yaw, roll));
	}

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;
};
