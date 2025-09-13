#include "AFContent.h"

#include <filesystem>

#include "AFAnimationClip.h"
#include "AFFloatCurve.h"
#include "AFMesh.h"
#include "AFPostprocessShader.h"
#include "AFVertexBuffer.h"
#include "AFTexture.h"
#include "AFStructs.h"
#include "AFShader.h"

AFContent& AFContent::Get()
{
	static AFContent content;
	return content;
}

bool AFContent::Init()
{
	// -------------------------------------------------
	// Custom made.
	// -------------------------------------------------

	// ---------------
	// Basic 100x100x100 units box.
	// ---------------

	std::shared_ptr<AFMesh> boxMesh = CreateAsset<AFMesh>();
	AFSubMesh boxSubMesh = AFSubMesh();

	// Front.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), 0, glm::vec2(0.125f, 0.875f)); // 0.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.75f), 0, glm::vec2(0.125f, 0.875f)); // 1.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.25f, 0.75f), 0, glm::vec2(0.125f, 0.875f)); // 2.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.25f, 1.0f), 0, glm::vec2(0.125f, 0.875f)); // 3.
	boxSubMesh.indices.emplace_back(0);
	boxSubMesh.indices.emplace_back(1);
	boxSubMesh.indices.emplace_back(2);
	boxSubMesh.indices.emplace_back(0);
	boxSubMesh.indices.emplace_back(2);
	boxSubMesh.indices.emplace_back(3);

	// Upper.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.25f, 1.0f), 1, glm::vec2(0.375f, 0.875f)); // 8.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.25f, 0.75f), 1, glm::vec2(0.375f, 0.875f)); // 9.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 0.75f), 1, glm::vec2(0.375f, 0.875f)); // 10.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f), 1, glm::vec2(0.375f, 0.875f)); // 11.
	boxSubMesh.indices.emplace_back(8);
	boxSubMesh.indices.emplace_back(9);
	boxSubMesh.indices.emplace_back(10);
	boxSubMesh.indices.emplace_back(8);
	boxSubMesh.indices.emplace_back(10);
	boxSubMesh.indices.emplace_back(11);

	// Behind.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.75f, 1.0f), 2, glm::vec2(0.875f, 0.875f)); // 12.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.75f, 0.75f), 2, glm::vec2(0.875f, 0.875f)); // 13.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.75f), 2, glm::vec2(0.875f, 0.875f)); // 14.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), 2, glm::vec2(0.875f, 0.875f)); // 15.
	boxSubMesh.indices.emplace_back(12);
	boxSubMesh.indices.emplace_back(13);
	boxSubMesh.indices.emplace_back(14);
	boxSubMesh.indices.emplace_back(12);
	boxSubMesh.indices.emplace_back(14);
	boxSubMesh.indices.emplace_back(15);

	// Under.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 1.0f), 3, glm::vec2(0.625f, 0.875f)); // 16.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.5f, 0.75f), 3, glm::vec2(0.625f, 0.875f)); // 17.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.75f, 0.75f), 3, glm::vec2(0.625f, 0.875f)); // 18.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.75f, 1.0f), 3, glm::vec2(0.625f, 0.875f)); // 19.
	boxSubMesh.indices.emplace_back(16);
	boxSubMesh.indices.emplace_back(17);
	boxSubMesh.indices.emplace_back(18);
	boxSubMesh.indices.emplace_back(16);
	boxSubMesh.indices.emplace_back(18);
	boxSubMesh.indices.emplace_back(19);

	// Right.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, 50.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.75f), 4, glm::vec2(0.375f, 0.625f)); // 4.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, 50.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.5f), 4, glm::vec2(0.375f, 0.625f)); // 5.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, -50.0f, -50.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.5f), 4, glm::vec2(0.375f, 0.625f)); // 6.
	boxSubMesh.vertices.emplace_back(glm::vec3(50.0f, 50.0f, -50.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.5f, 0.75f), 4, glm::vec2(0.375f, 0.625f)); // 7.
	boxSubMesh.indices.emplace_back(4);
	boxSubMesh.indices.emplace_back(5);
	boxSubMesh.indices.emplace_back(6);
	boxSubMesh.indices.emplace_back(4);
	boxSubMesh.indices.emplace_back(6);
	boxSubMesh.indices.emplace_back(7);

	// Left.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, -50.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.75f), 5, glm::vec2(0.125f, 0.625f)); // 20.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, -50.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.5f), 5, glm::vec2(0.125f, 0.625f)); // 21.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, -50.0f, 50.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.5f), 5, glm::vec2(0.125f, 0.625f)); // 22.
	boxSubMesh.vertices.emplace_back(glm::vec3(-50.0f, 50.0f, 50.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.25f, 0.75f), 5, glm::vec2(0.125f, 0.625f)); // 23.
	boxSubMesh.indices.emplace_back(20);
	boxSubMesh.indices.emplace_back(21);
	boxSubMesh.indices.emplace_back(22);
	boxSubMesh.indices.emplace_back(20);
	boxSubMesh.indices.emplace_back(22);
	boxSubMesh.indices.emplace_back(23);

	boxMesh->subMeshes.push_back(boxSubMesh);
	AddAsset("sm_box", boxMesh);

	// ---------------
	// Basic 2x2 units plane.
	// ---------------

	std::shared_ptr<AFMesh> planeMesh = CreateAsset<AFMesh>();
	AFSubMesh planeSubMesh = AFSubMesh();

	planeSubMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)); // 0.
	planeSubMesh.vertices.emplace_back(glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f)); // 1.
	planeSubMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f)); // 2.
	planeSubMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)); // 3.
	planeSubMesh.indices.emplace_back(0);
	planeSubMesh.indices.emplace_back(1);
	planeSubMesh.indices.emplace_back(2);
	planeSubMesh.indices.emplace_back(0);
	planeSubMesh.indices.emplace_back(2);
	planeSubMesh.indices.emplace_back(3);

	planeMesh->subMeshes.push_back(planeSubMesh);
	AddAsset("sm_plane", planeMesh);

	// ---------------
	// Three-directional gizmo.
	// ---------------

	std::shared_ptr<AFMesh> gizmoMesh = CreateAsset<AFMesh>();
	AFSubMesh gizmoSubMesh;

	gizmoSubMesh.vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f)); // 0 - Middle.
	gizmoSubMesh.vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f)); // 1 - Up.
	gizmoSubMesh.vertices.emplace_back(glm::vec3(1.0f, 0.0f, 0.0f)); // 2 - Right.
	gizmoSubMesh.vertices.emplace_back(glm::vec3(0.0f, 0.0f, 1.0f)); // 3 - Forward.
	gizmoSubMesh.indices.emplace_back(0);
	gizmoSubMesh.indices.emplace_back(1);
	gizmoSubMesh.indices.emplace_back(0);
	gizmoSubMesh.indices.emplace_back(2);
	gizmoSubMesh.indices.emplace_back(0);
	gizmoSubMesh.indices.emplace_back(3);
	gizmoSubMesh.vertexBuffer = std::make_shared<AFVertexBuffer>();
	gizmoSubMesh.vertexBuffer->SetDrawMode(GL_LINES);

	gizmoMesh->subMeshes.push_back(gizmoSubMesh);
	AddAsset("sm_gizmo", gizmoMesh);

	// -------------------------------------------------
	// Loaded from files.
	// -------------------------------------------------

	// Textures.
	std::shared_ptr<AFTexture> boxTex = AddAsset<AFTexture>("t_box", "content/textures/orientBox.png", true);
	std::shared_ptr<AFTexture> mannequinTex = AddAsset<AFTexture>("t_mannequin", "content/textures/mannequin.png", false);

	// Shaders.
	AddAsset<AFShader>("shader_background", "content/shaders/background.vert", "content/shaders/background.frag");
	AddAsset<AFShader>("shader_grid", "content/shaders/grid.vert", "content/shaders/grid.frag");
	AddAsset<AFShader>("shader_basic", "content/shaders/basic.vert", "content/shaders/basic.frag");
	AddAsset<AFShader>("shader_basicNoTex", "content/shaders/basic.vert", "content/shaders/basicNoTex.frag");
	AddAsset<AFShader>("shader_basicSkinned", "content/shaders/basicSkinned.vert", "content/shaders/basic.frag");
	AddAsset<AFShader>("shader_basicGLTF", "content/shaders/basicGLTF.vert", "content/shaders/basicGLTF.frag");
	AddAsset<AFShader>("shader_gizmo", "content/shaders/uiLocalRotateOrtho.vert", "content/shaders/orientationGizmo.frag");
	AddAsset<AFShader>("shader_glyph", "content/shaders/glyph.vert", "content/shaders/glyph.frag");
	AddAsset<AFShader>("shader_idPickerUI", "content/shaders/uiLocalRotate.vert", "content/shaders/idPicker.frag");
	AddAsset<AFShader>("shader_orientBox", "content/shaders/uiLocalRotate.vert", "content/shaders/orientationBox.frag");
	AddAsset<AFShader>("shader_stencil", "content/shaders/stencil.vert", "content/shaders/stencil.frag");
	AddAsset<AFShader>("shader_idPicker", "content/shaders/idPicker.vert", "content/shaders/idPicker.frag");
	AddAsset<AFShader>("shader_neutral", "content/shaders/neutral.vert", "content/shaders/neutral.frag");
	AddAsset<AFShader>("shader_idPickerVisualize", "content/shaders/neutral.vert", "content/shaders/idPickerVisualize.frag");
	AddAsset<AFShader>("shader_uiFullscreen", "content/shaders/neutral.vert", "content/shaders/uiFullscreen.frag");
	AddAsset<AFPostprocessShader>("shader_fisheye", "content/shaders/fisheye.vert", "content/shaders/fisheye.frag");
	AddAsset<AFPostprocessShader>("shader_gaussianHorizontal", "content/shaders/gaussianHorizontal.vert", "content/shaders/gaussianHorizontal.frag");
	AddAsset<AFPostprocessShader>("shader_gaussianVertical", "content/shaders/gaussianVertical.vert", "content/shaders/gaussianVertical.frag");

	// Meshes.
	std::shared_ptr<AFMesh> mannequin = AddAsset<AFMesh>("sk_mannequin", "content/models/sk_mannequin.gltf");

	// Curves.
	// Automatically load all the float curves from the content folder.
	for (const auto& file : std::filesystem::directory_iterator("content/curves"))
	{
		if (file.is_regular_file() && file.path().extension() == ".json")
		{
			const std::string filename = file.path().filename().filename().string();
			const std::string curveName = file.path().filename().stem().string();
			AddAsset<AFFloatCurve>(curveName.c_str(), ("content/curves/" + filename).c_str());
		}
	}

	// -------------------------------------------------
	// Apply fallback properties post load.
	// -------------------------------------------------

	// Mannequin.
	mannequin->subMeshes[0].texture = mannequinTex;

	// Box.
	boxMesh->subMeshes[0].texture = boxTex;


	return true;
}
