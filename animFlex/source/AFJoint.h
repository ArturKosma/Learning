#pragma once
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/detail/type_quat.hpp>

class AFJoint
{
public:

	void SetLocation(const glm::vec3& newLocation);
	void SetRotation(const glm::quat newRotation);
	void SetScale(const glm::vec3& newScale);

	glm::vec3 GetLocation() const;
	glm::quat GetRotation() const;
	glm::vec3 GetScale() const;

	static std::shared_ptr<AFJoint> CreateRoot(int rootBoneIdx);
	void AddChildren(const std::vector<int>& newChildBones);

	void CalculateLocalTRSMatrix();
	void CalculateNodeMatrix(const glm::mat4& parentNodeMatrix);

	glm::mat4 GetLocalTRSMatrix() const;
	glm::mat4 GetNodeMatrix() const;

	int GetNodeID() const;

	std::vector<std::shared_ptr<AFJoint>> GetChildren() const;

	void SetNodeName(const std::string& newName);
	std::string GetNodeName() const;

	void PrintTree() const;
	static void PrintNodes(std::shared_ptr<AFJoint> bone, int indent);

private:

	int nodeID = 0;
	std::string nodeName = {};

	glm::vec3 location = glm::vec3(0.0f);
	glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f);

	glm::mat4 localTRSMatrix = glm::mat4(1.0f);
	glm::mat4 nodeMatrix = glm::mat4(1.0f);

	std::vector<std::shared_ptr<AFJoint>> childNodes = {};
};
