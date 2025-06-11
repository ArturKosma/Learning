#pragma once
#include "AFStructs.h"

class AFSerializer
{
public:

	template<typename T>
	static bool Serialize(const char* targetPath, FAFAsset* sourceAsset);

	template<typename T>
	static bool Deserialize(const char* sourcePath, FAFAsset* targetAsset);
};

template <typename T>
bool AFSerializer::Serialize(const char* targetPath, FAFAsset* sourceAsset)
{
	if (!targetPath || !sourceAsset)
	{
		return false;
	}

	return sourceAsset->Serialize<T>(targetPath);
}

template <typename T>
bool AFSerializer::Deserialize(const char* sourcePath, FAFAsset* targetAsset)
{
	if (!sourcePath || !targetAsset)
	{
		return false;
	}

	return targetAsset->Deserialize<T>(sourcePath);
}
