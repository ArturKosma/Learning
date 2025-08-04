#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AFComponent.h"

class AFSceneComponent : public AFComponent
{
public:

	AFSceneComponent();
	virtual ~AFSceneComponent() override = default;

	glm::vec3 GetWorldLocation() const;
	glm::vec3 GetWorldRotation() const;
	glm::quat GetWorldRotationQuat() const;
	glm::vec3 GetWorldScale() const;
	glm::mat4 GetWorldTransform() const;

	glm::vec3 GetLocalLocation() const;
	glm::vec3 GetLocalRotation() const;
	glm::quat GetLocalRotationQuat() const;
	glm::vec3 GetLocalScale() const;
	glm::mat4 GetLocalTransform() const;

	void SetLocalLocation(const glm::vec3& newLocation);
	void SetLocalRotation(const glm::vec3& newEulerRotation);
	void SetLocalRotation(const glm::quat& newQuatRotation);
	void SetLocalScale(const glm::vec3& newScale);
	void SetLocalTransform(const glm::mat4& newTransform);

	void SetWorldLocation(const glm::vec3& newWorldLocation);
	void SetWorldTransform(const glm::mat4& newWorldTransform);

	void AddLocalOffsetLocation(const glm::vec3& offset);
	void AddLocalOffsetRotation(const glm::vec3& offset);
	void AddLocalOffsetScale(const glm::vec3& offset);

	virtual void OnOwnerTransform(glm::mat4 offset);

protected:

	void RecreateTransform();

	glm::vec3 m_localLocation = glm::vec3(0.0f);
	glm::quat m_localRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::vec3 m_localScale = glm::vec3(1.0f);
	glm::mat4 m_localTransform = glm::mat4();
};