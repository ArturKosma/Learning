#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>

#include "AFStructs.h"
#include "AFUtility.h"
#include "AFTexture.h"
#include "AFFloatCurve.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/fetch.h>
#endif

class AFContent
{
public:

	static AFContent& Get();
	bool Init();

	// Add new asset given it's class and filepath(s).
	template<typename T, typename... Args>
	std::shared_ptr<T> AddAsset(const char* assetName, Args&&... args);

	// Add existing asset.
	inline std::shared_ptr<AFAsset> AddAsset(const char* assetName, std::shared_ptr<AFAsset> existingAsset);

	// Find existing previously added asset.
	template<typename T>
	std::shared_ptr<T> FindAsset(const char* assetName) const;

	// Find existing previously added assets that match the part of the name.
	template<typename T>
	std::vector<std::shared_ptr<T>> FindAssets(const char* assetPartName) const;

	// Download file via jsdelivr.
	// Insta call onComplete if assetName exists in assets map.
	template<typename T>
	void FetchAsset(const std::string& directoryPath, const std::string& assetName, std::function<void(std::shared_ptr<T>)> onComplete);

private:

	AFContent() {}

	template<typename T>
	std::shared_ptr<T> CreateAsset();

	std::unordered_map<std::string, std::shared_ptr<AFAsset>> assets;
};

template <typename T>
std::shared_ptr<T> AFContent::CreateAsset()
{
	std::shared_ptr<T> newObject = std::make_shared<T>();
	std::shared_ptr<AFAsset> casted = std::static_pointer_cast<AFAsset>(newObject);

	casted->m_uniqueID = AFIDGenerator::Next();

	return newObject;
}

template <typename T, typename... Args>
std::shared_ptr<T> AFContent::AddAsset(const char* assetName, Args&&... args)
{
	static_assert(std::is_base_of<AFAsset, T>::value, "Added asset must derive from AFAsset.");

	// Enforce unique names.
	if(std::shared_ptr<T> existing = FindAsset<T>(assetName))
	{
		printf("Add asset warning. Asset with name %s already exists.\n", assetName);
		return existing;
	}

	std::shared_ptr<T> newAsset = std::make_shared<T>();
	std::shared_ptr<AFAsset> casted = std::dynamic_pointer_cast<AFAsset>(newAsset);
	if(casted->Load<T>(std::forward<Args>(args)...))
	{
		casted->m_name = assetName;
		casted->OnLoadComplete();
		assets.insert({ assetName , newAsset });
		return newAsset;
	}

	return nullptr;
}

template <typename T>
std::shared_ptr<T> AFContent::FindAsset(const char* assetName) const
{
	static_assert(std::is_base_of<AFAsset, T>::value, "Found asset must derive from AFAsset.");

	auto it = assets.find(assetName);
	if (it != assets.end())
	{
		return std::dynamic_pointer_cast<T>(it->second);
	}

	return nullptr;
}

template <typename T>
std::vector<std::shared_ptr<T>> AFContent::FindAssets(const char* assetPartName) const
{
	static_assert(std::is_base_of<AFAsset, T>::value, "Found asset must derive from AFAsset.");

	std::vector<std::shared_ptr<T>> ret;

	for (const auto& [name, ptr] : assets)
	{
		if (name.find(assetPartName) == std::string::npos)
		{
			continue;
		}

		std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(ptr);
		if (casted)
		{
			ret.push_back(casted);
		}
	}

	return ret;
}

template <typename T>
void AFContent::FetchAsset(const std::string& directoryPath, const std::string& assetName, std::function<void(std::shared_ptr<T>)> onComplete)
{
	if (assetName == "")
	{
		return;
	}

	std::shared_ptr<T> ret = std::make_shared<T>();

	const std::string& fullPath = directoryPath + assetName;

	auto it = assets.find(assetName);
	if (it != assets.end())
	{
		// Insta call complete.
		onComplete(std::dynamic_pointer_cast<T>(it->second));
		return;
	}

	// Fetch asset via jsdelivr.
#ifdef __EMSCRIPTEN__
	emscripten_fetch_attr_t attr;
	emscripten_fetch_attr_init(&attr);
	strcpy(attr.requestMethod, "GET");
	attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;

	FAFFetchContext<T>* usrcontext = new FAFFetchContext<T>
	{
		fullPath,
		assetName,
		ret,
		this,
		onComplete
	};
	attr.userData = usrcontext;

	attr.onsuccess = [](emscripten_fetch_t* fetch)
	{
		// Get passed user context.
		FAFFetchContext<T>* userContext = static_cast<FAFFetchContext<T>*>(fetch->userData);

		// Deserialize the data.
		userContext->ret->template Deserialize<T>(fetch->data, fetch->numBytes);

		// Assign name for the new asset.
		userContext->ret->m_name = userContext->assetName;

		// Call complete load on the object.
		userContext->ret->OnLoadComplete();

		// Cache the asset so it's easily obtainable.
		userContext->content->assets.insert({ userContext->assetName, userContext->ret });

		// Free fetch memory.
		emscripten_fetch_close(fetch);

		// Call complete.
		userContext->onComplete(userContext->ret);

		delete userContext;
	};

	attr.onerror = [](emscripten_fetch_t* fetch)
	{
		// Get passed user context.
		FAFFetchContext<T>* userContext = static_cast<FAFFetchContext<T>*>(fetch->userData);

		// Free fetch memory.
		emscripten_fetch_close(fetch);

		// Call complete.
		userContext->onComplete(userContext->ret);

		delete userContext;
	};

	emscripten_fetch(&attr, fullPath.c_str());

#endif
}

std::shared_ptr<AFAsset> AFContent::AddAsset(const char* assetName, std::shared_ptr<AFAsset> existingAsset)
{
	// Enforce unique names.
	if (std::shared_ptr<AFAsset> existing = FindAsset<AFAsset>(assetName))
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
