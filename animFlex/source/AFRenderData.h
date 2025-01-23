#pragma once

#include <vector>
#include <glm/glm.hpp>

struct AFRenderData
{
	struct GLFWwindow* window = nullptr;

	unsigned int width = 0;
	unsigned int height = 0;

	size_t vertexCount = 0;
	size_t triangleCount = 0;

	int fieldOfView = 90;

	float gameDrawTime = -1.0f;
	float uiDrawTime = -1.0f;
};

struct AFVertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 uv;
};

struct AFMesh
{
	std::vector<AFVertex> vertices;
};