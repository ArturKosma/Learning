#include "AFModel.h"

void AFModel::Init()
{
	m_vertexData.vertices.resize(36);

	// Triangles (model space).

	// Front square.
	m_vertexData.vertices[0].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[1].position = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[2].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	m_vertexData.vertices[3].position = glm::vec3(-0.5f,-0.5f, 0.5f);
	m_vertexData.vertices[4].position = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[5].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Behind square.
	m_vertexData.vertices[6].position = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[7].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[8].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	//
	m_vertexData.vertices[9].position = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[10].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[11].position = glm::vec3(0.5f, 0.5f, -0.5f);
	// Upper square.
	m_vertexData.vertices[12].position = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[13].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[14].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	m_vertexData.vertices[15].position = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[16].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[17].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Lower square.
	m_vertexData.vertices[18].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[19].position = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[20].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	//
	m_vertexData.vertices[21].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[22].position = glm::vec3(0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[23].position = glm::vec3(0.5f, -0.5f, 0.5f);
	// Left square.
	m_vertexData.vertices[24].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[25].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[26].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	//
	m_vertexData.vertices[27].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[28].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	m_vertexData.vertices[29].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	// Right square.
	m_vertexData.vertices[30].position = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[31].position = glm::vec3(0.5f, 0.5f, 0.5f);
	m_vertexData.vertices[32].position = glm::vec3(0.5f, -0.5f, 0.5f);
	//
	m_vertexData.vertices[33].position = glm::vec3(0.5f, 0.5f, -0.5f);
	m_vertexData.vertices[34].position = glm::vec3(0.5f, -0.5f, 0.5f);
	m_vertexData.vertices[35].position = glm::vec3(0.5f, -0.5f, -0.5f);

	// Colors.

	// Front square.
	m_vertexData.vertices[0].color = glm::vec3(1.0f, 0.0f, 0.0f); // Red.
	m_vertexData.vertices[1].color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData.vertices[2].color = glm::vec3(1.0f, 0.0f, 0.0f);
	//
	m_vertexData.vertices[3].color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData.vertices[4].color = glm::vec3(1.0f, 0.0f, 0.0f);
	m_vertexData.vertices[5].color = glm::vec3(1.0f, 0.0f, 0.0f);
	// Behind square.
	m_vertexData.vertices[6].color = glm::vec3(0.0f, 1.0f, 0.0f); // Green.
	m_vertexData.vertices[7].color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData.vertices[8].color = glm::vec3(0.0f, 1.0f, 0.0f);
	//
	m_vertexData.vertices[9].color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData.vertices[10].color = glm::vec3(0.0f, 1.0f, 0.0f);
	m_vertexData.vertices[11].color = glm::vec3(0.0f, 1.0f, 0.0f);
	// Upper square.
	m_vertexData.vertices[12].color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue.
	m_vertexData.vertices[13].color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData.vertices[14].color = glm::vec3(0.0f, 0.0f, 1.0f);
	//
	m_vertexData.vertices[15].color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData.vertices[16].color = glm::vec3(0.0f, 0.0f, 1.0f);
	m_vertexData.vertices[17].color = glm::vec3(0.0f, 0.0f, 1.0f);
	// Lower square.
	m_vertexData.vertices[18].color = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow.
	m_vertexData.vertices[19].color = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertexData.vertices[20].color = glm::vec3(1.0f, 1.0f, 0.0f);
	//
	m_vertexData.vertices[21].color = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertexData.vertices[22].color = glm::vec3(1.0f, 1.0f, 0.0f);
	m_vertexData.vertices[23].color = glm::vec3(1.0f, 1.0f, 0.0f);
	// Left square.
	m_vertexData.vertices[24].color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan.
	m_vertexData.vertices[25].color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_vertexData.vertices[26].color = glm::vec3(0.0f, 1.0f, 1.0f);
	//
	m_vertexData.vertices[27].color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_vertexData.vertices[28].color = glm::vec3(0.0f, 1.0f, 1.0f);
	m_vertexData.vertices[29].color = glm::vec3(0.0f, 1.0f, 1.0f);
	// Right square.
	m_vertexData.vertices[30].color = glm::vec3(1.0f, 0.0f, 1.0f); // Purple.
	m_vertexData.vertices[31].color = glm::vec3(1.0f, 0.0f, 1.0f);
	m_vertexData.vertices[32].color = glm::vec3(1.0f, 0.0f, 1.0f);
	//											
	m_vertexData.vertices[33].color = glm::vec3(1.0f, 0.0f, 1.0f);
	m_vertexData.vertices[34].color = glm::vec3(1.0f, 0.0f, 1.0f);
	m_vertexData.vertices[35].color = glm::vec3(1.0f, 0.0f, 1.0f);

	// UV.

	// Front square.
	m_vertexData.vertices[0].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	m_vertexData.vertices[1].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	m_vertexData.vertices[2].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	//
	m_vertexData.vertices[3].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	m_vertexData.vertices[4].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	m_vertexData.vertices[5].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	// Behind square.
	m_vertexData.vertices[6].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[7].uv = glm::vec2(1.0f, 0.0f);
	m_vertexData.vertices[8].uv = glm::vec2(1.0f, 1.0f);
	//
	m_vertexData.vertices[9].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[10].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[11].uv = glm::vec2(0.0f, 1.0f);
	// Upper square.
	m_vertexData.vertices[12].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[13].uv = glm::vec2(1.0f, 0.0f);
	m_vertexData.vertices[14].uv = glm::vec2(1.0f, 1.0f);
	//
	m_vertexData.vertices[15].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[16].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[17].uv = glm::vec2(0.0f, 1.0f);
	// Lower square.
	m_vertexData.vertices[18].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[19].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[20].uv = glm::vec2(1.0f, 0.0f);
	//
	m_vertexData.vertices[21].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[22].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[23].uv = glm::vec2(0.0f, 1.0f);
	// Left square.
	m_vertexData.vertices[24].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[25].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[26].uv = glm::vec2(1.0f, 0.0f);
	//
	m_vertexData.vertices[27].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[28].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[29].uv = glm::vec2(0.0f, 1.0f);
	// Right square.
	m_vertexData.vertices[30].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[31].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[32].uv = glm::vec2(1.0f, 0.0f);
	//
	m_vertexData.vertices[33].uv = glm::vec2(0.0f, 0.0f);
	m_vertexData.vertices[34].uv = glm::vec2(1.0f, 1.0f);
	m_vertexData.vertices[35].uv = glm::vec2(0.0f, 1.0f);
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
