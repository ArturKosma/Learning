#pragma once

#include <vector>
#include <glm/glm.hpp>

struct AFCameraProperties
{
	int fieldOfView = 60;
};

struct AFSceneData
{
	std::vector<class AFActor*> sceneActors = std::vector<class AFActor*>();
	int sceneTriangles = 0;
	class AFCamera* activeCamera = nullptr;
};

struct AFAppData
{
	struct GLFWwindow* window = nullptr;
	int width = 0;
	int height = 0;
};

struct AFVertex
{
	AFVertex() = default;
	AFVertex(const glm::vec3 newPosition)
		: position(newPosition), color(glm::vec3(0.0f)), uv(glm::vec2(0.0f))
	{
		
	}
	AFVertex(const glm::vec3& newPosition, const glm::vec3& newColor, const glm::vec2& newUV)
		: position(newPosition), color(newColor), uv(newUV)
	{

	}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 uv;
};

struct AFMesh
{
	std::vector<AFVertex> vertices;
};
