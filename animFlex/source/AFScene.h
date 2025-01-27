#pragma once

#include "AFActor.h"
#include "AFStructs.h"

class AFScene
{
	friend class AFGame;

public:

	void AddActor(AFActor* newActor);

	const AFSceneData& GetSceneData() const;

private:

	bool Init();
	void CreateDefaultSceneActors();

	AFScene();
	~AFScene();

	AFSceneData m_sceneData;
};
