#pragma once
#include <string>
#include "AFStructs.h"

class AFGLTFLoader
{
public:

	static bool Load(const std::string& filename, FAFMeshLoaded& loadedMesh, bool binary = false);
};
