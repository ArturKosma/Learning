#pragma once
#include <string>
#include "AFStructs.h"

class AFGLTFLoader
{
public:

	static AFMeshLoaded Load(const std::string& filename, bool binary = false);
};
