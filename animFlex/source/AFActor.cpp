#include "AFActor.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

AFActor::AFActor()
{
    RecreateTransform();
}

AFActor::~AFActor()
{

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

void AFActor::SetTransform(const glm::mat4& newTransform)
{
    m_transform = newTransform;

    glm::vec3 skew;
    glm::vec4 perspective;
    glm::vec3 scale;
    glm::quat rotation;
    glm::vec3 translation;
    
    glm::decompose(m_transform, scale, rotation, translation, skew, perspective);

    m_scale = scale;
    m_rotation = rotation;
    m_location = translation;
}

void AFActor::AddOffsetLocation(const glm::vec3& offset)
{
    SetLocation(m_location + offset);
}

void AFActor::AddOffsetRotation(const glm::vec3& offset)
{
    glm::quat quatToAdd = glm::quat(glm::radians(offset));

    SetRotation(quatToAdd * m_rotation);
}

void AFActor::AddOffsetScale(const glm::vec3& offset)
{
    SetScale(m_scale + offset);
}

void AFActor::RecreateTransform()
{
    OnTransformRecreation();

    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_location);
    glm::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);

    m_transform = translationMatrix * rotationMatrix * scaleMatrix;
}

void AFActor::OnTransformRecreation()
{

}
