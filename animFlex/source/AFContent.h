#pragma once
#include <fstream>
#include <memory>
#include <string>
#include <unordered_map>
#include <type_traits>

#include "AFStructs.h"
#include "AFUtility.h"
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
	template<typename T>
	std::shared_ptr<T> AddAsset(const char* assetName, std::shared_ptr<T> existingAsset);

	// Find existing previously added asset.
	template<typename T>
	std::shared_ptr<T> FindAsset(const char* assetName) const;

	// Find existing previously added assets that match the part of the name.
	template<typename T>
	std::vector<std::shared_ptr<T>> FindAssets(const char* assetPartName) const;

	// Find all assets of a given type.
	template <class T>
	std::vector<std::shared_ptr<T>> FindAllAssetsOfType() const;

	// Download file via jsdelivr.
	// Insta call onComplete if assetName exists in assets map.
	template<typename T>
	void FetchAsset(const std::string& directoryPath, const std::string& assetName, std::function<void(std::shared_ptr<T>)> onComplete, const std::string& extension = "");

	// Downloads all files from jsdelivr in a given folder,
	// if those files contain assetPartName.
	// Requires a local .json manifest which contains all possible names.
	template<typename T>
	void FetchAssets(const std::string& jsonManifest, const std::string& assetsDirectory, const std::string& assetPartName, std::function<void(std::vector<std::shared_ptr<T>>)> onComplete, const std::string& extension = "");

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
std::shared_ptr<T> AFContent::AddAsset(const char* assetName, std::shared_ptr<T> existingAsset)
{
	// Enforce unique names.
	if (std::shared_ptr<T> existing = FindAsset<T>(assetName))
	{
		//printf("Add asset warning. Asset with name %s already exists.\n", assetName);
		return existing;
	}

	if (existingAsset->LoadExisting())
	{
		existingAsset->m_name = assetName;
		assets.insert({ assetName , existingAsset });


		existingAsset->OnLoadComplete();

		return existingAsset;
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
	static_assert(std::is_base_of<AFAsset, T>::value, "T must derive from AFAsset.");

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
std::vector<std::shared_ptr<T>> AFContent::FindAllAssetsOfType() const
{
	static_assert(std::is_base_of_v<AFAsset, T>, "T must derive from AFAsset.");

	std::vector<std::shared_ptr<T>> ret;

	for (const auto& [name, ptr] : assets)
	{
		std::shared_ptr<T> casted = std::dynamic_pointer_cast<T>(ptr);
		if (casted)
		{
			ret.push_back(casted);
		}
	}

	return ret;
}

template <typename T>
void AFContent::FetchAsset(const std::string& directoryPath, const std::string& assetName, std::function<void(std::shared_ptr<T>)> onComplete, const std::string& extension)
{
	if (assetName == "")
	{
		return;
	}

	std::shared_ptr<T> ret = std::make_shared<T>();

	const std::string& fullPath = directoryPath + assetName + extension;

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

		// Make sure the asset wasn't added to content in the meantime.
		std::shared_ptr<T> existing = userContext->content->template FindAsset<T>(userContext->assetName.c_str());
		if (existing)
		{
			// Free fetch memory.
			emscripten_fetch_close(fetch);

			// Call complete.
			userContext->onComplete(existing);

			delete userContext;

			return;
		}

		// Deserialize the data.
		userContext->ret->template Deserialize<T>(fetch->data, fetch->numBytes);

		// Add new asset to the content.
		userContext->content->AddAsset(userContext->assetName.c_str(), userContext->ret);

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
		userContext->onComplete(nullptr);

		delete userContext;
	};

	emscripten_fetch(&attr, fullPath.c_str());

#endif
}

template <typename T>
void AFContent::FetchAssets(const std::string& jsonManifest, const std::string& assetsDirectory, const std::string& assetPartName,
	std::function<void(std::vector<std::shared_ptr<T>>)> onComplete, const std::string& extension)
{
	if (assetPartName == "")
	{
		return;
	}

	// Batch context.
	struct BatchCtx {
		AFContent* self;
		std::function<void(std::vector<std::shared_ptr<T>>)> onComplete;
		std::vector<std::shared_ptr<T>> results;
		size_t remaining = 0;
	};

	// Functor to call when filenames list get fetched.
	auto fetchAllFileNames = [assetPartName, this, onComplete, assetsDirectory, extension](std::vector<std::string> filenames)
		{
			std::vector<std::string> matching;
			for (const std::string& filename : filenames)
			{
				auto it = filename.find(assetPartName);
				if (it == std::string::npos)
				{
					continue;
				}

				// Add matching filenames after the list fetch.
				matching.push_back(filename);
			}

			// Prepare batch context.
			BatchCtx* bctx = new BatchCtx{ this, onComplete, {}, matching.size() };
			if (bctx->remaining == 0) 
			{
				bctx->onComplete(std::move(bctx->results));
				delete bctx;
				return;
			}

			for (const std::string& filename : matching)
			{
				// If this specific file already exists in content, don't fetch it.
				auto it = assets.find(filename);
				if (it != assets.end())
				{
					bctx->results.push_back(std::dynamic_pointer_cast<T>(it->second));

					if (--bctx->remaining == 0)
					{
						bctx->onComplete(std::move(bctx->results));
						return;
					}

					continue;
				}

				const std::string fullUrl = assetsDirectory + filename + extension;

				// Per-request context.
				struct PerReq
				{
					BatchCtx* batch;
					std::string assetName;
					std::shared_ptr<T> ret;
				};

				// Prepare request.
				auto* req = new PerReq
				{
					bctx,
					filename,
					std::make_shared<T>()
				};

#ifdef __EMSCRIPTEN__
				emscripten_fetch_attr_t attr;
				emscripten_fetch_attr_init(&attr);
				std::strcpy(attr.requestMethod, "GET");
				attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
				attr.userData = req;

				attr.onsuccess = [](emscripten_fetch_t* fetch)
				{
					// Pointer to single request.
					auto* req = static_cast<PerReq*>(fetch->userData);

					// Pointer to batch data.
					auto* batch = req->batch;

					// Make sure the asset wasn't added to content in the meantime.
					std::shared_ptr<T> existing = batch->self->template FindAsset<T>(req->assetName.c_str());
					if (existing)
					{

						batch->results.push_back(existing);

						// Close fetch for this single file.
						emscripten_fetch_close(fetch);

						// Complete full batch?
						if (--batch->remaining == 0)
						{
							// Call complete.
							batch->onComplete(std::move(batch->results));
							delete batch;
						}

						delete req;
						return;
					}

					// Deserialize downloaded asset.
					req->ret->template Deserialize<T>(fetch->data, fetch->numBytes);

					// Add deserialized asset to content.
					batch->self->AddAsset(req->assetName.c_str(), req->ret);

					// Close fetch for this single file.
					emscripten_fetch_close(fetch);

					// Add deserialized result to the batch.
					batch->results.push_back(req->ret);

					// Complete full batch?
					if (--batch->remaining == 0) 
					{
						// Call complete.
						batch->onComplete(std::move(batch->results));
						delete batch;
					}

					delete req;

				};
				attr.onerror = [](emscripten_fetch_t* fetch)
				{
					// Pointer to single request.
					auto* req = static_cast<PerReq*>(fetch->userData);

					// Pointer to batch data.
					auto* batch = req->batch;

					// Close fetch.
					emscripten_fetch_close(fetch);

					if (--batch->remaining == 0)
					{
						// Call complete.
						batch->onComplete(std::move(batch->results));
						delete batch;
					}

					delete req;
				};

				// Request single file.
				emscripten_fetch(&attr, fullUrl.c_str());
#endif
			}
		};

	// Open json manifest.
	std::vector<std::string> names;
	std::ifstream in(jsonManifest);
	nlohmann::json data = nlohmann::json::parse(in); // Array.
	names.reserve(data.size());

	// Add every name to the array.
	for (auto& item : data)
	{
		if (item.contains("name") && item["name"].is_string())
		{
			names.emplace_back(item["name"].get<std::string>());
		}
	}

	fetchAllFileNames(names);
}
