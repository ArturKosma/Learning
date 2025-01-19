#pragma once

#include "AFRenderData.h"

#include <vector>
#include <glm/glm.hpp>

class AFModel
{
	friend class AFWindow;

public:

	void Init();
	AFMesh GetVertexData() const;

private:

	AFModel();
	~AFModel();

	AFMesh m_vertexData = {};
};