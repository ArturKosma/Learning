#include "AFBoxComponent.h"

bool AFBoxComponent::Load()
{
	AFMesh boxMesh = AFMesh();

	/*
	// Front square.
	testBoxMesh.vertices[0].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[1].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[2].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	testBoxMesh.vertices[3].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[4].position = glm::vec3(0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[5].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Behind square.
	testBoxMesh.vertices[6].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[7].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[8].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	//
	testBoxMesh.vertices[9].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[10].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[11].position = glm::vec3(0.5f, 0.5f, -0.5f);
	// Upper square.
	testBoxMesh.vertices[12].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[13].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[14].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	testBoxMesh.vertices[15].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[16].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[17].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Lower square.
	testBoxMesh.vertices[18].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[19].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[20].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	//
	testBoxMesh.vertices[21].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[22].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[23].position = glm::vec3(0.5f, -0.5f, 0.5f);
	// Left square.
	testBoxMesh.vertices[24].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[25].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[26].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	//
	testBoxMesh.vertices[27].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[28].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[29].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	// Right square.
	testBoxMesh.vertices[30].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[31].position = glm::vec3(0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[32].position = glm::vec3(0.5f, -0.5f, 0.5f);
	//
	testBoxMesh.vertices[33].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[34].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[35].position = glm::vec3(0.5f, -0.5f, -0.5f);

	// Colors.

	// Front square.
	testBoxMesh.vertices[0].color = glm::vec3(1.0f, 0.0f, 0.0f); // Red.
	testBoxMesh.vertices[1].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[2].color = glm::vec3(1.0f, 0.0f, 0.0f);
	//
	testBoxMesh.vertices[3].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[4].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[5].color = glm::vec3(1.0f, 0.0f, 0.0f);
	// Behind square.
	testBoxMesh.vertices[6].color = glm::vec3(0.0f, 1.0f, 0.0f); // Green.
	testBoxMesh.vertices[7].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[8].color = glm::vec3(0.0f, 1.0f, 0.0f);
	//
	testBoxMesh.vertices[9].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[10].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[11].color = glm::vec3(0.0f, 1.0f, 0.0f);
	// Upper square.
	testBoxMesh.vertices[12].color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue.
	testBoxMesh.vertices[13].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[14].color = glm::vec3(0.0f, 0.0f, 1.0f);
	//
	testBoxMesh.vertices[15].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[16].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[17].color = glm::vec3(0.0f, 0.0f, 1.0f);
	// Lower square.
	testBoxMesh.vertices[18].color = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow.
	testBoxMesh.vertices[19].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[20].color = glm::vec3(1.0f, 1.0f, 0.0f);
	//
	testBoxMesh.vertices[21].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[22].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[23].color = glm::vec3(1.0f, 1.0f, 0.0f);
	// Left square.
	testBoxMesh.vertices[24].color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan.
	testBoxMesh.vertices[25].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[26].color = glm::vec3(0.0f, 1.0f, 1.0f);
	//
	testBoxMesh.vertices[27].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[28].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[29].color = glm::vec3(0.0f, 1.0f, 1.0f);
	// Right square.
	testBoxMesh.vertices[30].color = glm::vec3(1.0f, 0.0f, 1.0f); // Purple.
	testBoxMesh.vertices[31].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[32].color = glm::vec3(1.0f, 0.0f, 1.0f);
	//											
	testBoxMesh.vertices[33].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[34].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[35].color = glm::vec3(1.0f, 0.0f, 1.0f);

	// UV.

	// Front square.
	testBoxMesh.vertices[0].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[1].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[2].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	//
	testBoxMesh.vertices[3].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[4].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[5].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	// Behind square.
	testBoxMesh.vertices[6].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[7].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[8].uv = glm::vec2(1.0f, 1.0f);
	//
	testBoxMesh.vertices[9].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[10].uv = glm::vec2(1.0f, 1.0f);
	testBoxMesh.vertices[11].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	// Upper square.
	testBoxMesh.vertices[12].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[13].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[14].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	//
	testBoxMesh.vertices[15].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[16].uv = glm::vec2(0.0f, 1.0f);
	testBoxMesh.vertices[17].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	// Lower square.
	testBoxMesh.vertices[18].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[19].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[20].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	//
	testBoxMesh.vertices[21].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[22].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[23].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	// Left square.
	testBoxMesh.vertices[24].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[25].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[26].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);
	//
	testBoxMesh.vertices[27].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[28].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[29].uv = glm::vec2(1.0f, 1.0f / 3.0f);
	// Right square.
	testBoxMesh.vertices[30].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[31].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[32].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	//
	testBoxMesh.vertices[33].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[34].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[35].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);
	*/

	// Front.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec2(0.0f, 1.0f)); // 0.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec2(0.0f, 0.0f)); // 1.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, 50.0f), glm::vec2(1.0f, 0.0f)); // 2.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec2(1.0f, 1.0f)); // 3.
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(3);

	// Upper.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec2(0.0f, 0.0f)); // 4.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec2(1.0f, 0.0f)); // 5.
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(0);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(4);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(5);

	// Behind.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec2(0.0f, 1.0f)); // 6.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec2(1.0f, 1.0f)); // 7.
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(5);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(4);

	// Under.
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(6);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(7);
	boxMesh.indices.emplace_back(2);

	// Right.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec2(0.0f, 0.0f)); // 8.
	boxMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec2(0.0f, 1.0f)); // 9.
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(2);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(3);
	boxMesh.indices.emplace_back(8);
	boxMesh.indices.emplace_back(9);

	// Left.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec2(1.0f, 1.0f)); // 10.
	boxMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec2(1.0f, 0.0f)); // 11.
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(11);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(10);
	boxMesh.indices.emplace_back(1);
	boxMesh.indices.emplace_back(0);

	m_mesh = boxMesh;

	return AFStaticMeshComponent::Load();
}
