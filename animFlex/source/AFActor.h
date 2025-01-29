#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AFComponent.h"

class AFActor
{
public:

	AFActor();
	virtual ~AFActor();

	virtual void Tick(float deltaTime);

	void AddComponent(AFComponent* newComponent);
	const std::vector<AFComponent*>& GetComponents() const;

	glm::vec3 GetLocation() const;
	glm::vec3 GetRotation() const;
	glm::quat GetRotationQuat() const;
	glm::vec3 GetScale() const;
	glm::mat4 GetTransform() const;

	void SetLocation(const glm::vec3& newLocation);
	void SetRotation(const glm::vec3& newEulerRotation);
	void SetRotation(const glm::quat& newQuatRotation);
	void SetScale(const glm::vec3& newScale);

	void AddOffsetLocation(const glm::vec3& offset);
	void AddOffsetRotation(const glm::vec3& offset);
	void AddOffsetScale(const glm::vec3& offset);

protected:

	std::vector<AFComponent*> m_components = std::vector<AFComponent*>();

	void RecreateTransform();

	glm::vec3 m_location = glm::vec3(0.0f);
	glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::mat4 m_transform = glm::mat4();
};
