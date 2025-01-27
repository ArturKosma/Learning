#include "AFHelperInterface.h"
#include "AFTimerManager.h"
#include "AFCamera.h"

#include <string>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "AFWindow.h"

bool AFHelperInferface::Init(const class AFWindow& window)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.GetGLFWWindow(), true);
	const char* glslVersion = "#version 300 es";
	ImGui_ImplOpenGL3_Init(glslVersion);

	return true;
}

void AFHelperInferface::Draw(const AFAppData& appData, const AFSceneData& sceneData)
{
	CreateFrame(appData, sceneData);
	Render();
}

bool AFHelperInferface::CreateFrame(const AFAppData& appData, const AFSceneData& sceneData)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiWindowFlags imguiWindowFlags = ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::SetNextWindowBgAlpha(0.8f);
	ImGui::Begin("Control", nullptr, imguiWindowFlags);

	ImGui::Text("FPS:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(1.0f / AFTimerManager::GetDeltaTime()).c_str());

	ImGui::Separator();

	ImGui::Text("Triangles:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(sceneData.triangles).c_str());

	std::string windowDims = std::to_string(appData.width) + "x" + std::to_string(appData.height);
	ImGui::Text("Window Dimensions:");
	ImGui::SameLine();
	ImGui::Text("%s", windowDims.c_str());

	ImGui::Separator();

	ImGui::Checkbox("Test State", &m_checkBox_testState);
	if(m_checkBox_testState)
	{
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
		ImGui::Text("Yes");
		ImGui::PopStyleColor();
	}

	ImGui::Separator();

	const AFCamera* const camera = sceneData.activeCamera;
	if(!camera)
	{
		ImGui::End();
		return false;
	}

	ImGui::Text("Field of View");
	ImGui::SameLine();
	int tempFOV = camera->GetCameraComponent()->GetCameraProperties().fieldOfView;
	if(ImGui::SliderInt("##FOV", &tempFOV, 40, 150))
	{
		
	}

	ImGui::Separator();

	ImGui::Text("Camera Pos:");
	ImGui::SameLine();
	const std::string& posStr = std::to_string(camera->GetLocation().x) + ", " +
		std::to_string(camera->GetLocation().y) + ", " + std::to_string(camera->GetLocation().z);
	ImGui::Text("%s", posStr.c_str());
	ImGui::Text("Camera Rot:");
	ImGui::SameLine();
	const std::string& rotStr = std::to_string(camera->GetRotation().x) + ", " +
		std::to_string(camera->GetRotation().y) + ", " + std::to_string(camera->GetRotation().z);
	ImGui::Text("%s", rotStr.c_str());
	ImGui::Text("Camera Speed:");
	/*ImGui::SameLine();
	const std::string& camspeed = std::to_string(camera.GetCameraSpeedMultiplier());
	ImGui::Text("%s", camspeed.c_str());*/

	ImGui::End();

	return true;
}

void AFHelperInferface::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AFHelperInferface::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
