#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>

#include "AFStructs.h"
#include "AFUtility.h"
#include "AFShader.h"
#include "AFTexture.h"

class AFContent
{
public:

	static AFContent& Get();
	bool Init();

	// Add new asset given it's class and filepath(s).
	template<typename T, typename... Args>
	std::shared_ptr<T> AddAsset(const char* assetName, Args&&... args);

	// Add existing asset.
	inline std::shared_ptr<FAFAsset> AddAsset(const char* assetName, std::shared_ptr<FAFAsset> existingAsset);

	// Find existing previously added asset.
	template<typename T>
	std::shared_ptr<T> FindAsset(const char* assetName) const;

private:

	AFContent() {}

	template<typename T>
	std::shared_ptr<T> CreateAsset();

	std::unordered_map<std::string, std::shared_ptr<FAFAsset>> assets;
};

template <typename T>
std::shared_ptr<T> AFContent::CreateAsset()
{
	std::shared_ptr<T> newObject = std::make_shared<T>();
	std::shared_ptr<FAFAsset> casted = std::static_pointer_cast<FAFAsset>(newObject);

	casted->m_uniqueID = AFIDGenerator::Next();

	return newObject;
}

template <typename T, typename... Args>
std::shared_ptr<T> AFContent::AddAsset(const char* assetName, Args&&... args)
{
	static_assert(std::is_base_of<FAFAsset, T>::value, "Added asset must derive from FAFAsset.");

	// Enforce unique names.
	if(std::shared_ptr<T> existing = FindAsset<T>(assetName))
	{
		printf("Add asset warning. Asset with name %s already exists.\n", assetName);
		return existing;
	}

	std::shared_ptr<T> newAsset = std::make_shared<T>();
	std::shared_ptr<FAFAsset> casted = std::dynamic_pointer_cast<FAFAsset>(newAsset);
	if(casted->Load<T>(std::forward<Args>(args)...))
	{
		assets.insert({ assetName , newAsset });
		return newAsset;
	}

	return nullptr;
}

template <typename T>
std::shared_ptr<T> AFContent::FindAsset(const char* assetName) const
{
	static_assert(std::is_base_of<FAFAsset, T>::value, "Found asset must derive from FAFAsset.");

	auto it = assets.find(assetName);
	if (it != assets.end())
	{
		return std::dynamic_pointer_cast<T>(it->second);
	}

	return nullptr;
}

std::shared_ptr<FAFAsset> AFContent::AddAsset(const char* assetName, std::shared_ptr<FAFAsset> existingAsset)
{
	// Enforce unique names.
	if (std::shared_ptr<FAFAsset> existing = FindAsset<FAFAsset>(assetName))
	{
		printf("Add asset warning. Asset with name %s already exists.\n", assetName);
		return existing;
	}

	if (existingAsset->LoadExisting())
	{
		assets.insert({ assetName , existingAsset });
		return existingAsset;
	}

	return nullptr;
}
