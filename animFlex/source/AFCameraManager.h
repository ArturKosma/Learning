#pragma once
#include "AFCamera.h"
#include "AFScene.h"

class AFCameraManager
{
public:

	void Init(const AFScene& scene);
	void BeginPlay();

	void UpdateState();
	void SetActiveCamera(std::shared_ptr<AFCamera> otherCamera);
	std::shared_ptr<AFCamera> GetActiveCamera() const;

	void BlendTo(std::shared_ptr<AFCameraComponent> otherCamera, float length, EAFInterpolationType interpType);
	void BlendToStatic(const glm::vec3& targetLocation, float targetPitch, float targetYaw, float length);

	void AddYaw(float yaw);
	void AddYawStroke(float yaw);
	void AddPitch(float pitch);
	void AddPitchStroke(float pitch);
	void ForwardBackward(float axis);
	void RightLeft(float axis);
	void UpDown(float axis);
	void ZoomStroke(float axis);
	void AddCameraSpeedMultiplier(float value);

private:

	bool m_blending = false;

	std::shared_ptr<AFCamera> m_activeCamera = nullptr;
	std::shared_ptr<AFPlayerPawn> m_playerPawn = nullptr;
};
