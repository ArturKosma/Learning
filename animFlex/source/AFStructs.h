#pragma once

#include <vector>

struct AFCameraProperties
{
	int fieldOfView = 90;
};

struct AFSceneData
{
	std::vector<class AFActor*> sceneActors = std::vector<class AFActor*>();
	int sceneTriangles = 0;
};

struct AFAppData
{
	const class GLFWwindow* window = nullptr;
	class AFCamera* activeCamera = nullptr;
	int triangles = 0;
	int width = 0;
	int height = 0;
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
