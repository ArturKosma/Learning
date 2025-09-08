#pragma once
#include <vector>
#include "AFAsset.h"
#include "AFJoint.h"
#include "AFStructs.h"

class AFSubMesh
{
public:

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

class AFMesh : public AFAsset
{
public:

	std::vector<AFSubMesh> subMeshes = {};

	bool LoadExisting() override;
	bool LoadImpl(const char* filepath) override;

	unsigned long long GetVertexCount() const;

	void RecalculateSkeleton();

	const std::vector<std::shared_ptr<AFJoint>>& GetJoints() const;
	glm::vec3 GetGlobalJointLocation(int index) const;
	glm::quat GetGlobalJointRotation(int index) const;

	bool jointsDirty = false;

	std::shared_ptr<AFJoint> rootJoint = nullptr;
	std::vector<int> nodeToJoint = {};
	std::vector<std::shared_ptr<AFJoint>> joints = {};
	std::vector<glm::mat4> inverseBindMatrices = {};
	std::vector<glm::mat4> jointMatrices = {};
	std::vector<glm::mat4> jointDualQuats = {};
};