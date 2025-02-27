#pragma once

#include <vector>
#include <glm/glm.hpp>

struct AFCameraProperties
{
	int fieldOfView = 60;
	float near = 1.0f;
	float far = 200.0f;
};

struct AFSceneData
{
	std::vector<class AFActor*> sceneActors = std::vector<class AFActor*>();
	std::vector<class AFUI*> uis = std::vector<class AFUI*>();
	unsigned long long vertexCount = 0;
	class AFCamera* activeCamera = nullptr;
	class AFStaticMeshComponent* background = nullptr;
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
		: position(newPosition), uv(glm::vec2(0.0f))
	{
		
	}
	AFVertex(const glm::vec3& newPosition, const glm::vec2& newUV)
		: position(newPosition), uv(newUV)
	{

	}

	glm::vec3 position;
	glm::vec2 uv;
};

struct AFMesh
{
	std::vector<AFVertex> vertices;
	std::vector<unsigned int> indices;
};
