#include "AFAsset.h"

unsigned int AFAsset::GetUniqueID() const
{
	return m_uniqueID;
}

bool AFAsset::LoadExisting()
{
	printf("load existing base\n");
	return false;
}