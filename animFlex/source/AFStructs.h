#pragma once

#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>

#include "third_party/tiny_gltf.h"

struct FAFCameraProperties
{
	int fieldOfView = 60;
	float near = 20.0f;
	float far = 20000.0f;
};

struct FAFSceneData
{
	std::vector<std::shared_ptr<class AFActor>> sceneActors = {};
	std::vector<std::shared_ptr<class AFUI>> uis = {};
	unsigned long long vertexCount = 0;
	std::shared_ptr<class AFCamera> activeCamera = nullptr;
};

struct FAFVertex
{
	FAFVertex() = default;
	FAFVertex(const glm::vec3 newPosition)
		: position(newPosition), normal(glm::vec3(0.0f)), uv(glm::vec2(0.0f)), faceID(0), uvCenter(glm::vec2(0.0f))
	{
		
	}
	FAFVertex(const glm::vec3& newPosition, const glm::vec3& newNormal, const glm::vec2& newUV, glm::uint8_t newFaceID = 0, glm::vec2 newUVCenter = glm::vec2())
		: position(newPosition), normal(newNormal), uv(newUV), faceID(newFaceID), uvCenter(newUVCenter)
	{

	}

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::uint8_t faceID;
	glm::vec2 uvCenter;
};

struct FAFAsset
{
	friend class AFContent;

	virtual ~FAFAsset() = default;

	unsigned int GetUniqueID() const;

	template<typename T, typename... Args>
	bool Load(Args&&... args);

	virtual bool LoadExisting();

protected:

	virtual bool LoadImpl(const char* filepath)
	{
		printf("loadimpl base char*\n");
		return false;
	}
	virtual bool LoadImpl(const char* filepath1, const char* filepath2)
	{
		printf("loadimpl base char* char*\n");
		return false;
	}
	virtual bool LoadImpl(const char* filepath, bool boolean)
	{
		printf("loadimpl base char* bool\n");
		return false;
	}

	unsigned int m_uniqueID = 0;
};

template <typename T, typename ... Args>
bool FAFAsset::Load(Args&&... args)
{
	return LoadImpl(std::forward<Args>(args)...);
}

struct AFBone
{
public:

	void SetLocation(const glm::vec3& newLocation);
	void SetRotation(const glm::quat newRotation);
	void SetScale(const glm::vec3& newScale);

	static std::shared_ptr<AFBone> CreateRoot(int rootBoneIdx);
	void AddChildren(const std::vector<int>& newChildBones);

	void CalculateLocalTRSMatrix();
	void CalculateNodeMatrix(const glm::mat4& parentNodeMatrix);

	glm::mat4 GetBoneMatrix() const;

	int GetBoneID() const;

	std::vector<std::shared_ptr<AFBone>> GetChildren() const;

	void SetBoneName(const std::string& newName);
	std::string GetBoneName() const;

	void PrintTree() const;
	static void PrintNodes(std::shared_ptr<AFBone> bone, int indent);

private:

	int boneID = 0;
	std::string boneName = {};

	glm::vec3 location = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 localTRSMatrix = glm::mat4(1.0f);
	glm::mat4 boneMatrix = glm::mat4(1.0f);
	glm::mat4 inverseBindMatrix = glm::mat4(1.0f);

	std::vector<std::shared_ptr<AFBone>> childBones = {};
};

struct FAFSubMesh
{
	std::vector<FAFVertex> vertices = {};
	std::vector<unsigned int> indices = {};

	std::shared_ptr<class AFVertexBuffer> vertexBuffer = nullptr;
	std::shared_ptr<class AFTexture> texture = nullptr;
	std::shared_ptr<class AFShader> shader = nullptr;

	bool depthTest = true;
	bool stencilTest = true;

	// Various additional info that might help identifying this sub-mesh.
	// For example a single character defining what glyph this is in text rendering.
	std::string metaInformation = {};

	unsigned long long GetVertexCount() const;
};

struct FAFMesh : public FAFAsset
{
	std::vector<FAFSubMesh> subMeshes = {};

	bool LoadExisting() override;
	bool LoadImpl(const char* filepath, bool binary) override;

	unsigned long long GetVertexCount() const;

	std::shared_ptr<AFBone> rootbone = nullptr;
};

struct FAFSubMeshLoaded
{
	GLuint vao;
	GLuint drawMode;
	GLsizei drawCount;
	GLenum drawType;
};

struct FAFMeshLoaded
{
	std::vector<FAFSubMeshLoaded> subMeshesLoaded;
	std::shared_ptr<AFBone> rootbone = nullptr;
};

enum class EAFDrawType : uint8_t
{
	Normal,
	IDPicker
};

struct FAFAppData
{
	struct GLFWwindow* window = nullptr;
	int width = 0;
	int height = 0;
	EAFDrawType drawType = EAFDrawType::Normal;
};

struct FAFDrawOverride
{
	EAFDrawType drawType;
	std::shared_ptr<class AFShader> shader;
};

struct FAFPickID
{
	uint32_t objectId = 0;
	uint8_t elementId = 0;
};

enum class EAFCameraMovementMode : uint8_t
{
	Normal,
	Blending
};

enum class EAFAlphaInterp : uint8_t
{
	Linear,
	CubicHermite
};

struct FAFGlyph
{
	unsigned int textureId = 0;
	glm::ivec2 size = glm::ivec2(0);
	glm::ivec2 bearing = glm::ivec2(0);
	unsigned int advance = 0;
};
