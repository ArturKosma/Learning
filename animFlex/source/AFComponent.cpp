#include "AFComponent.h"
#include "AFActor.h"

AFComponent::AFComponent()
{
    RecreateTransform();
}

AFComponent::~AFComponent()
{
}

void AFComponent::Tick(float deltaTime)
{

}

void AFComponent::SetOwner(AFActor* newOwner)
{
    m_owner = newOwner;
}

AFActor* AFComponent::GetOwner() const
{
	return m_owner;
}

glm::vec3 AFComponent::GetWorldLocation() const
{
    AFActor* owner = GetOwner();
    if(!owner)
    {
        return glm::vec3(0.0f);
    }

    return m_localLocation + owner->GetLocation();
}

glm::vec3 AFComponent::GetWorldRotation() const
{
    AFActor* owner = GetOwner();
    if (!owner)
    {
        return glm::vec3(0.0f);
    }

    return glm::degrees(glm::eulerAngles(m_localRotation * owner->GetRotationQuat()));
}

glm::quat AFComponent::GetWorldRotationQuat() const
{
    AFActor* owner = GetOwner();
    if (!owner)
    {
        return glm::quat();
    }

    return m_localRotation * owner->GetRotationQuat();
}

glm::vec3 AFComponent::GetWorldScale() const
{
    AFActor* owner = GetOwner();
    if (!owner)
    {
        return glm::vec3(1.0f);
    }

    return m_localScale * owner->GetScale();
}

glm::mat4 AFComponent::GetWorldTransform() const
{
    AFActor* owner = GetOwner();
    if (!owner)
    {
        return glm::mat4();
    }

    return m_localTransform * owner->GetTransform();
}

glm::vec3 AFComponent::GetLocalLocation() const
{
    return m_localLocation;
}

glm::vec3 AFComponent::GetLocalRotation() const
{
    return glm::degrees(glm::eulerAngles(m_localRotation));
}

glm::quat AFComponent::GetLocalRotationQuat() const
{
    return m_localRotation;
}

glm::vec3 AFComponent::GetLocalScale() const
{
    return m_localScale;
}

glm::mat4 AFComponent::GetLocalTransform() const
{
    return m_localTransform;
}

void AFComponent::SetLocalLocation(const glm::vec3& newLocation)
{
    m_localLocation = newLocation;

    RecreateTransform();
}

void AFComponent::SetLocalRotation(const glm::vec3& newEulerRotation)
{
    m_localRotation = glm::quat(glm::radians(newEulerRotation));

    RecreateTransform();
}

void AFComponent::SetLocalRotation(const glm::quat& newQuatRotation)
{
    m_localRotation = newQuatRotation;

    RecreateTransform();
}

void AFComponent::SetLocalScale(const glm::vec3& newScale)
{
    m_localScale = newScale;

    RecreateTransform();
}

void AFComponent::AddLocalOffsetLocation(const glm::vec3& offset)
{
    SetLocalRotation(m_localLocation + offset);
}

void AFComponent::AddLocalOffsetRotation(const glm::vec3& offset)
{
    glm::quat quatToAdd = glm::quat(offset);

    SetLocalRotation(quatToAdd * m_localRotation);
}

void AFComponent::AddLocalOffsetScale(const glm::vec3& offset)
{
    SetLocalScale(m_localScale + offset);
}

void AFComponent::RecreateTransform()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_localLocation);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_localRotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_localScale);

    m_localTransform = translationMatrix * rotationMatrix * scaleMatrix;
}
