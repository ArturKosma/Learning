#include "AFModel.h"

void AFModel::Init()
{
	m_vertexData.vertices.resize(6);

	m_vertexData.vertices[0].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[1].position = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[2].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[3].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[4].position = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[5].position = glm::vec3(0.5f, 0.5f, 0.5f);

	m_vertexData.vertices[0].color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData.vertices[1].color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_vertexData.vertices[2].color = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertexData.vertices[3].color = glm::vec3(1.0f, 0.0f, 1.0f);
	m_vertexData.vertices[4].color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData.vertices[5].color = glm::vec3(1.0f, 1.0f, 1.0f);

	m_vertexData.vertices[0].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[1].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[2].uv = glm::vec2(0.0f, 1.0f);
	m_vertexData.vertices[3].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[4].uv = glm::vec2(1.0f, 0.0f);
	m_vertexData.vertices[5].uv = glm::vec2(1.0f, 1.0f);
}

AFMesh AFModel::GetVertexData() const
{
	return m_vertexData;
}

AFModel::AFModel()
{

}

AFModel::~AFModel()
{

}
