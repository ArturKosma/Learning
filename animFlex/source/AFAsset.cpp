#include "AFAsset.h"

unsigned int AFAsset::GetUniqueID() const
{
	return m_uniqueID;
}

std::string AFAsset::GetName() const
{
	return m_name;
}

bool AFAsset::LoadExisting()
{
	printf("load existing base\n");
	return false;
}

void AFAsset::OnLoadComplete()
{
}
