#pragma once
#include "AFAsset.h"

class AFSerializer
{
public:

	template<typename T>
	static bool Serialize(const char* targetPath, AFAsset* sourceAsset);

	template<typename T>
	static bool Deserialize(const char* sourcePath, AFAsset* targetAsset);
};

template <typename T>
bool AFSerializer::Serialize(const char* targetPath, AFAsset* sourceAsset)
{
	if (!targetPath || !sourceAsset)
	{
		return false;
	}

	return sourceAsset->Serialize<T>(targetPath);
}

template <typename T>
bool AFSerializer::Deserialize(const char* sourcePath, AFAsset* targetAsset)
{
	if (!sourcePath || !targetAsset)
	{
		return false;
	}

	return targetAsset->Deserialize<T>(sourcePath);
}
