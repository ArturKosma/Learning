#pragma once

#include "AFActor.h"
#include "AFStructs.h"
#include "AFUtility.h"

class AFScene
{
	friend class AFGame;

public:

	void AddActor(AFActor* newActor);
	void AddUI(AFUI* newUI);

	void SetActiveCamera(class AFCamera* newActiveCamera);
	AFCamera* GetActiveCamera() const;

	const AFSceneData& GetSceneData() const;

	template<typename T>
	static T* CreateObject();

private:

	unsigned int m_idCounter = 0;

	bool Init();
	void CreateDefaultSceneActors();
	void CreateDefaultUIs();

	AFScene();
	~AFScene();

	AFSceneData m_sceneData;
};

template <typename T>
T* AFScene::CreateObject()
{
	T* newObject = new T;
	AFObject* casted = static_cast<AFObject*>(newObject);

	casted->m_uniqueId = AFIDGenerator::Next();

	return newObject;
}
