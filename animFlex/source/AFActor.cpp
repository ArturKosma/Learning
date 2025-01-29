#include "AFActor.h"

AFActor::AFActor()
{
}

AFActor::~AFActor()
{

}

void AFActor::Tick(float deltaTime)
{
    for(AFComponent* comp : m_components)
    {
	    if(!comp)
	    {
            continue;
	    }
        comp->Tick(deltaTime);
    }
}

void AFActor::AddComponent(AFComponent* newComponent)
{
    newComponent->SetOwner(this);
    m_components.push_back(newComponent);
}

const std::vector<AFComponent*>& AFActor::GetComponents() const
{
    return m_components;
}

glm::vec3 AFActor::GetLocation() const
{
    return m_location;
}

glm::vec3 AFActor::GetRotation() const
{
    return glm::degrees(glm::eulerAngles(m_rotation));
}

glm::quat AFActor::GetRotationQuat() const
{
    return m_rotation;
}

glm::vec3 AFActor::GetScale() const
{
    return m_scale;
}

glm::mat4 AFActor::GetTransform() const
{
    return m_transform;
}

void AFActor::SetLocation(const glm::vec3& newLocation)
{
    m_location = newLocation;

    RecreateTransform();
}

void AFActor::SetRotation(const glm::vec3& newEulerRotation)
{
    m_rotation = glm::quat(glm::radians(newEulerRotation));

    RecreateTransform();
}

void AFActor::SetRotation(const glm::quat& newQuatRotation)
{
    m_rotation = newQuatRotation;

    RecreateTransform();
}

void AFActor::SetScale(const glm::vec3& newScale)
{
    m_scale = newScale;

    RecreateTransform();
}

void AFActor::AddOffsetLocation(const glm::vec3& offset)
{
    SetLocation(m_location + offset);
}

void AFActor::AddOffsetRotation(const glm::vec3& offset)
{
    glm::quat quatToAdd = glm::quat(offset);

    SetRotation(quatToAdd * m_rotation);
}

void AFActor::AddOffsetScale(const glm::vec3& offset)
{
    SetScale(m_scale + offset);
}

void AFActor::RecreateTransform()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_location);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

    m_transform = translationMatrix * rotationMatrix * scaleMatrix;
}
