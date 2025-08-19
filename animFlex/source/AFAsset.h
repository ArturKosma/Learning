#pragma once
#include <cstdio>
#include <string>
#include <utility>

class AFAsset
{
	friend class AFContent;

public:

	virtual ~AFAsset() = default;

	unsigned int GetUniqueID() const;
	std::string GetName() const;

	template<typename T, typename... Args>
	bool Load(Args&&... args);

	template<typename T, typename... Args>
	bool Serialize(Args&&... args);

	template<typename T, typename... Args>
	bool Deserialize(Args&&... args);

	virtual bool LoadExisting();

	virtual void OnLoadComplete();

protected:

	virtual bool LoadImpl(const char* filepath)
	{
		printf("loadimpl base char*\n");
		return false;
	}
	virtual bool LoadImpl(const char* filepath1, const char* filepath2)
	{
		printf("loadimpl base char* char*\n");
		return false;
	}
	virtual bool LoadImpl(const char* filepath, bool boolean)
	{
		printf("loadimpl base char* bool\n");
		return false;
	}

	virtual bool SerializeImpl(const char* filepath)
	{
		printf("serializeImpl base char*\n");
		return false;
	}

	virtual bool DeserializeImpl(const char* filepath)
	{
		printf("deserializeImpl base char*\n");
		return false;
	}

	virtual bool DeserializeImpl()
	{
		printf("deserializeImpl base\n");
		return false;
	}

	unsigned int m_uniqueID = 0;
	std::string m_name = "";
};

template <typename T, typename ... Args>
bool AFAsset::Load(Args&&... args)
{
	return LoadImpl(std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
bool AFAsset::Serialize(Args&&... args)
{
	return SerializeImpl(std::forward<Args>(args)...);
}

template <typename T, typename ... Args>
bool AFAsset::Deserialize(Args&&... args)
{
	return DeserializeImpl(std::forward<Args>(args)...);
}