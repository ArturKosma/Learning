#pragma once
#include <string>

enum class EAFSyncGroupMode;

class IAFSyncGroupInterface
{
public:

	virtual ~IAFSyncGroupInterface() = default;

	virtual std::string GetSyncGroupName() = 0;
	virtual EAFSyncGroupMode GetSyncGroupMode() = 0;
	virtual float GetLocalTime() = 0;
	virtual AFAnimationClip* GetAnimClip() = 0;
};
