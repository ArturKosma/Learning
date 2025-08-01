#pragma once

#include "AFActor.h"
#include "AFStructs.h"
#include "AFUtility.h"

class AFScene
{
	friend class AFGame;

public:

	void BeginPlay();

	std::shared_ptr<AFActor> FindActor(const std::string& actorName) const;
	void AddActor(std::shared_ptr<AFActor> newActor);
	void AddUI(std::shared_ptr<AFUI> newUI);

	void SetPlayerPawn(std::shared_ptr<AFPlayerPawn> newPlayerPawn);
	std::shared_ptr<AFPlayerPawn> GetPlayerPawn() const;

	const FAFSceneData& GetSceneData() const;

	template<typename T>
	static std::shared_ptr<T> CreateObject();

private:

	unsigned int m_idCounter = 0;

	bool Init();
	void CreateDefaultSceneActors();
	void CreateDefaultUIs();

	AFScene();
	~AFScene();

	FAFSceneData m_sceneData;
};

template <typename T>
std::shared_ptr<T> AFScene::CreateObject()
{
	std::shared_ptr<T> newObject = std::make_shared<T>();
	std::shared_ptr<AFObject> casted = std::static_pointer_cast<AFObject>(newObject);

	casted->m_uniqueId = AFIDGenerator::Next();

	std::shared_ptr<AFComponentOwner> compOwner = std::dynamic_pointer_cast<AFComponentOwner>(casted);
	if (compOwner)
	{
		// If components were added in the construction script of the compOwner,
		// set their owner.
		compOwner->RegisterComponents();
	}

	return newObject;
}
