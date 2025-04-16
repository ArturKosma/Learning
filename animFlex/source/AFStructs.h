#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "AFShader.h"

struct AFCameraProperties
{
	int fieldOfView = 60;
	float near = 20.0f;
	float far = 20000.0f;
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
		: position(newPosition), uv(glm::vec2(0.0f)), uniqueId(glm::vec4(0))
	{
		
	}
	AFVertex(const glm::vec3& newPosition, const glm::vec2& newUV, const glm::vec4& newUniqueID = glm::vec4(0.0f))
		: position(newPosition), uv(newUV), uniqueId(newUniqueID)
	{

	}

	glm::vec3 position;
	glm::vec2 uv;
	glm::u8vec4 uniqueId;
};

struct AFMesh
{
	std::vector<AFVertex> vertices;
	std::vector<unsigned int> indices;
};

enum class EAFDrawType : uint8_t
{
	Normal,
	IDPicker
};

struct AFDrawOverride
{
	EAFDrawType drawType;
	AFShader shader;
};

struct FAFPickID
{
	uint32_t objectId = 0;
	uint8_t elementId = 0;
};
