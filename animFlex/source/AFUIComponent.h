#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AFComponent.h"

class AFUIComponent : public AFComponent
{
public:

	AFUIComponent();
	virtual ~AFUIComponent() = default;

	glm::vec2 GetLocation() const;
	glm::vec2 GetScale() const;

	glm::mat4 GetUITransform() const;

	void SetLocation(const glm::vec2& newLocation);
	void SetScale(const glm::vec2& newScale);

	void AddOffsetLocation(const glm::vec2& offset);

protected:

	void RecreateTransform();

	glm::vec2 m_location = glm::vec2(0.0f);
	glm::vec2 m_scale = glm::vec2(1.0f);

	glm::mat4 m_uiTransform = glm::mat4();
};