#include "AFUserInterface.h"
#include "AFWindow.h"

#include <string>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void AFUserInferface::Init(const AFRenderData& renderData)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(renderData.window, true);
	const char* glslVersion = "#version 300 es";
	ImGui_ImplOpenGL3_Init(glslVersion);
}

void AFUserInferface::CreateFrame(AFRenderData& renderData)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags imguiWindowFlags = ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::Begin("Control", nullptr, imguiWindowFlags);

	ImGui::Text("FPS:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(1.0f / AFWindow::deltaTime).c_str());

	ImGui::Separator();

	ImGui::Text("Game Draw Time:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(renderData.gameDrawTime).c_str());
	ImGui::SameLine();
	ImGui::Text("ms");

	ImGui::Text("UI Draw Time:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(renderData.uiDrawTime).c_str());
	ImGui::SameLine();
	ImGui::Text("ms");

	ImGui::Separator();

	ImGui::Text("Vertices:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(renderData.vertexCount).c_str());

	ImGui::Text("Triangles:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(renderData.triangleCount).c_str());

	std::string windowDims = std::to_string(renderData.width) + "x" + std::to_string(renderData.height);
	ImGui::Text("Window Dimensions:");
	ImGui::SameLine();
	ImGui::Text("%s", windowDims.c_str());

	std::string imgWindowPos = std::to_string(static_cast<int>(ImGui::GetWindowPos().x)) + "/" + std::to_string(static_cast<int>(ImGui::GetWindowPos().y));
	ImGui::Text("ImGui Window Position:");
	ImGui::SameLine();
	ImGui::Text("%s", imgWindowPos.c_str());

	ImGui::Separator();

	ImGui::Checkbox("Test State", &m_checkBox_testState);
	if(m_checkBox_testState)
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Yes");
		ImGui::PopStyleColor();
	}

	AFGame& game = AFGame::GetInstance();
	if(ImGui::Button("Toggle Shader"))
	{
		game.ToggleTestState();
	}
	ImGui::SameLine();
	if(!game.GetTestState())
	{
		ImGui::Text("Basic Shader");
	}
	else
	{
		ImGui::Text("Changed Shader");
	}

	ImGui::Separator();

	ImGui::Text("Field of View");
	ImGui::SameLine();
	ImGui::SliderInt("##FOV", &renderData.fieldOfView, 40, 150);

	ImGui::Separator();

	const AFCamera& camera = game.GetCamera();
	ImGui::Text("Camera Pos:");
	ImGui::SameLine();
	const std::string& posStr = std::to_string(camera.GetPosition().x) + ", " + 
		std::to_string(camera.GetPosition().y) + ", " + std::to_string(camera.GetPosition().z);
	ImGui::Text("%s", posStr.c_str());
	ImGui::Text("Camera Rot:");
	ImGui::SameLine();
	const std::string& rotStr = std::to_string(camera.GetRotationEuler().x) + ", " +
		std::to_string(camera.GetRotationEuler().y) + ", " + std::to_string(camera.GetRotationEuler().z);
	ImGui::Text("%s", rotStr.c_str());

	ImGui::End();
}

void AFUserInferface::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AFUserInferface::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
