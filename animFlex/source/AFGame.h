#pragma once
#include "AFCameraManager.h"
#include "AFScene.h"

class AFGame
{
	friend class AFApp;

public:

	bool Init();
	void Tick(float newDeltaTime);
	void BeginPlay();

	static AFGame* GetGame();
	const AFScene& GetScene() const;
	AFCameraManager* GetCameraManager() const;

	// #hack
	// Signals from the frontend probably shouldn't be here.
	// @todo fixme.
	void OnNodeCreated(const char* msg);
	void OnNodeUpdated(const char* msg);
	void OnNodeRemoved(const char* msg);
	void OnStateConnectionCreated(const char* msg);
	void OnStateConnectionRemoved(const char* msg);

	void SetControlMode(EAFControlMode newControlMode);
	EAFControlMode GetControlMode() const;

private:

	void OnSelect(const FAFPickID& pickID);
	void OnHover(const FAFPickID& pickID);

	void SetEditorControlMode(EAFEditorControlMode newEditorControlMode);

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();
	AFCameraManager* m_cameraManager = nullptr;

	std::shared_ptr<AFObject> m_currentHover = nullptr;
	uint8_t m_currentHoverElement = -1;

	EAFControlMode m_currentControlMode = EAFControlMode::ActionRPG;
	EAFEditorControlMode m_currentEditorControlMode = EAFEditorControlMode::Normal;
};