#include "AFUIComponent.h"

AFUIComponent::AFUIComponent()
	: AFComponent()
{
	RecreateTransform();
}

glm::vec2 AFUIComponent::GetLocation() const
{
	return m_location;
}

glm::vec2 AFUIComponent::GetScale() const
{
	return m_scale;
}

glm::mat4 AFUIComponent::GetUITransform() const
{
	return m_uiTransform;
}

void AFUIComponent::SetLocation(const glm::vec2& newLocation)
{
	m_location = newLocation;

	RecreateTransform();
}

void AFUIComponent::SetScale(const glm::vec2& newScale)
{
	m_scale = newScale;

	RecreateTransform();
}

void AFUIComponent::AddOffsetLocation(const glm::vec2& offset)
{
	SetLocation(GetLocation() + offset);
}

void AFUIComponent::RecreateTransform()
{
	glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(GetLocation(), 0.0f));
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(GetScale(), 1.0f));

	m_uiTransform = translationMatrix * rotationMatrix * scaleMatrix;
}
