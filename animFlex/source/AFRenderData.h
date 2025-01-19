#pragma once

#include <vector>
#include <glm/glm.hpp>

struct AFVertex
{
	glm::vec3 position;
	glm::vec2 uv;
};

struct AFMesh
{
	std::vector<AFVertex> vertices;
};