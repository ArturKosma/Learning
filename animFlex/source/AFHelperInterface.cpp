#include "AFHelperInterface.h"
#include "AFTimerManager.h"
#include "AFCamera.h"

#include <string>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "AFInput.h"
#include "AFUtility.h"
#include "AFWindow.h"

bool AFHelperInterface::Init(const class AFWindow& window)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window.GetGLFWWindow(), false);
	const char* glslVersion = "#version 300 es";
	ImGui_ImplOpenGL3_Init(glslVersion);

	// ImGui does something behind the scenes which makes my
	// emscripten_request_pointerlock insta exit.
	// This code prevents it.
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	return true;
}

void AFHelperInterface::Draw(const FAFSceneData& sceneData, FAFAppData& appData)
{
	ImGuiIO& io = ImGui::GetIO();

	// Manually inject events to ImGUI due to having touch events which are not supported.
	const bool bIsMobile = AFUtility::IsMobile();
	if (bIsMobile)
	{
		const bool touchDown = AFInput::GetInstance().GetTouchDown(0);
		const glm::vec2& touchPos = AFInput::GetInstance().GetTouchPos(0);

		io.MouseDown[0] = touchDown;
		io.MousePos = { touchPos.x, touchPos.y };
	}
	else
	{
		const bool mouseDown = AFInput::GetInstance().GetMouseDown();
		const glm::vec2& cursorPos = AFInput::GetInstance().GetCursorPos();

		io.MouseDown[0] = mouseDown;
		io.MousePos = { cursorPos.x, cursorPos.y };
	}

	CreateFrame(sceneData, appData);
	Render();
}

bool AFHelperInterface::CreateFrame(const FAFSceneData& sceneData, FAFAppData& appData)
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

	/*ImGui::Text("Vertex Count:");
	ImGui::SameLine();
	ImGui::Text("%s", std::to_string(sceneData.vertexCount).c_str());*/

	std::string windowDims = std::to_string(appData.width) + "x" + std::to_string(appData.height);
	ImGui::Text("Window Dimensions:");
	ImGui::SameLine();
	ImGui::Text("%s", windowDims.c_str());

	ImGui::Separator();

	ImGui::Text("Field of View");
	ImGui::SameLine();
	int tempFOV = sceneData.activeCamera->GetCameraProperties().fieldOfView;
	if(ImGui::SliderInt("##FOV", &tempFOV, 40, 150))
	{
		sceneData.activeCamera->SetFieldOfView(tempFOV);
	}

	ImGui::Separator();

	ImGui::Text("Camera Pos:");

	std::shared_ptr<AFCamera> cam = std::dynamic_pointer_cast<AFCamera>(sceneData.activeCamera->GetOwner().lock());
	if (!cam)
	{
		return false;
	}

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	const std::string& posX = std::to_string(cam->GetLocation().x);
	ImGui::Text("%s", posX.c_str());
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	const std::string& posY = std::to_string(cam->GetLocation().y);
	ImGui::Text("%s", posY.c_str());
	ImGui::PopStyleColor();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));

	ImGui::SameLine();
	const std::string& posZ = std::to_string(cam->GetLocation().z);
	ImGui::Text("%s", posZ.c_str());
	ImGui::PopStyleColor();

	ImGui::Text("Camera Rot:");
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	const std::string& rotPitch = std::to_string(cam->GetRotation().x);
	ImGui::Text("%s", rotPitch.c_str());
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
	const std::string& rotYaw = std::to_string(cam->GetRotation().y);
	ImGui::Text("%s", rotYaw.c_str());
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
	const std::string& rotRoll = std::to_string(cam->GetRotation().z);
	ImGui::Text("%s", rotRoll.c_str());
	ImGui::PopStyleColor();

	ImGui::Text("Camera Speed:");
	ImGui::SameLine();
	const std::string& camspeed = std::to_string(cam->GetMovementComponent()->GetCameraSpeedMultiplier());
	ImGui::Text("%s", camspeed.c_str());

	ImGui::Separator();

	// Select draw type.

	const char* renderingTypeStrings[] = { "Normal", "IDPicker" };
	int tmpDrawTypeSelect = static_cast<int>(appData.drawType);
	if(ImGui::Combo("Draw Option", &tmpDrawTypeSelect, renderingTypeStrings, 2))
	{
		appData.drawType = static_cast<EAFDrawType>(tmpDrawTypeSelect);
	}

	ImGui::End();

	return true;
}

void AFHelperInterface::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AFHelperInterface::Cleanup()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
