#include "AFSceneComponent.h"
#include "AFActor.h"

AFSceneComponent::AFSceneComponent()
{
	RecreateTransform();
}

glm::vec3 AFSceneComponent::GetWorldLocation() const
{
    AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
    if (!owner)
    {
        return glm::vec3(0.0f);
    }

    return m_localLocation + owner->GetLocation();
}

glm::vec3 AFSceneComponent::GetWorldRotation() const
{
    AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
    if (!owner)
    {
        return glm::vec3(0.0f);
    }

    return glm::degrees(glm::eulerAngles(m_localRotation * owner->GetRotationQuat()));
}

glm::quat AFSceneComponent::GetWorldRotationQuat() const
{
    AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
    if (!owner)
    {
        return glm::quat();
    }

    return m_localRotation * owner->GetRotationQuat();
}

glm::vec3 AFSceneComponent::GetWorldScale() const
{
    AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
    if (!owner)
    {
        return glm::vec3(1.0f);
    }

    return m_localScale * owner->GetScale();
}

glm::mat4 AFSceneComponent::GetWorldTransform() const
{
    AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
    if (!owner)
    {
        return glm::mat4();
    }

    return owner->GetTransform() * m_localTransform;
}

glm::vec3 AFSceneComponent::GetLocalLocation() const
{
    return m_localLocation;
}

glm::vec3 AFSceneComponent::GetLocalRotation() const
{
    return glm::degrees(glm::eulerAngles(m_localRotation));
}

glm::quat AFSceneComponent::GetLocalRotationQuat() const
{
    return m_localRotation;
}

glm::vec3 AFSceneComponent::GetLocalScale() const
{
    return m_localScale;
}

glm::mat4 AFSceneComponent::GetLocalTransform() const
{
    return m_localTransform;
}

void AFSceneComponent::SetLocalLocation(const glm::vec3& newLocation)
{
    m_localLocation = newLocation;

    RecreateTransform();
}

void AFSceneComponent::SetLocalRotation(const glm::vec3& newEulerRotation)
{
    m_localRotation = glm::quat(glm::radians(newEulerRotation));

    RecreateTransform();
}

void AFSceneComponent::SetLocalRotation(const glm::quat& newQuatRotation)
{
    m_localRotation = newQuatRotation;

    RecreateTransform();
}

void AFSceneComponent::SetLocalScale(const glm::vec3& newScale)
{
    m_localScale = newScale;

    RecreateTransform();
}

void AFSceneComponent::AddLocalOffsetLocation(const glm::vec3& offset)
{
    SetLocalRotation(m_localLocation + offset);
}

void AFSceneComponent::AddLocalOffsetRotation(const glm::vec3& offset)
{
    glm::quat quatToAdd = glm::quat(offset);

    SetLocalRotation(quatToAdd * m_localRotation);
}

void AFSceneComponent::AddLocalOffsetScale(const glm::vec3& offset)
{
    SetLocalScale(m_localScale + offset);
}

void AFSceneComponent::RecreateTransform()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_localLocation);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_localRotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_localScale);

    m_localTransform = translationMatrix * rotationMatrix * scaleMatrix;
}
