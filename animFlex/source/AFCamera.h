#pragma once

#include "AFActor.h"
#include "AFCameraComponent.h"
#include "AFCameraMovementComponent.h"

class AFCamera : public AFActor
{
public:

	AFCamera();

	AFCameraComponent* GetCameraComponent() const;
	AFCameraMovementComponent* GetMovementComponent() const;

private:

	void BindInputs();

	void Input_FreeView_CameraYaw(float delta);
	void Input_FreeView_CameraPitch(float delta);

	void Input_FreeView_CameraYaw_Stroke(float delta);
	void Input_FreeView_CameraPitch_Stroke(float delta);

	void Input_FreeView_ForwardBackward(float axis);
	void Input_FreeView_RightLeft(float axis);
	void Input_FreeView_UpDown(float axis);

	void Input_FreeView_Zoom_Stroke(float axis);

	AFCameraMovementComponent* m_movementComponent = nullptr;
	AFCameraComponent* m_cameraComponent = nullptr;

	float m_cameraRotStrength = 5.0f;

	// #hack
	// @todo Spit out a slider for mobile.
	float m_cameraRotStrength_stroke = 10.0f;
};
