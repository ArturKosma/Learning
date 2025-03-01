#pragma once

#include "AFActor.h"
#include "AFStructs.h"

class AFScene
{
	friend class AFGame;

public:

	void AddActor(AFActor* newActor);
	void AddUI(AFUI* newUI);

	void SetActiveCamera(class AFCamera* newActiveCamera);
	AFCamera* GetActiveCamera() const;

	const AFSceneData& GetSceneData() const;

private:

	bool Init();
	void CreateDefaultSceneActors();
	void CreateDefaultUIs();

	AFScene();
	~AFScene();

	AFSceneData m_sceneData;
};
