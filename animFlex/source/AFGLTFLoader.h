#pragma once
#include <string>

#include "AFAnimationClip.h"
#include "AFStructs.h"

class AFGLTFLoader
{
public:

	static bool Load(const std::string& filename, FAFMeshLoaded& loadedMesh, bool binary = false);

	static bool LoadAnim(const std::string& filename, AFAnimationClip* loadedClip, bool binary = false);
};
