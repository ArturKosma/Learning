#pragma once

#include <string>
#include <unordered_map>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "third_party/json.hpp"

#include "AFStructs.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

class AFPose;

class AFUtility
{
public:

	AFUtility() = delete;
	~AFUtility() = delete;

	static std::unordered_map<std::string, int> GetConfigurableKeys()
	{
		std::unordered_map<std::string, int> keymap =
		{
			{"KEY_W", GLFW_KEY_W},
			{"KEY_A", GLFW_KEY_A},
			{"KEY_S", GLFW_KEY_S},
			{"KEY_D", GLFW_KEY_D},
			{"KEY_Q", GLFW_KEY_Q},
			{"KEY_E", GLFW_KEY_E},
			{"KEY_TAB", GLFW_KEY_TAB},
			{"MOUSE_BUTTON_RIGHT", GLFW_MOUSE_BUTTON_RIGHT},
			{"MOUSE_BUTTON_LEFT", GLFW_MOUSE_BUTTON_LEFT},
			{"MOUSE_TURN_RIGHT", 1001},
			{"MOUSE_TURN_LEFT", 1002},
			{"MOUSE_TILT_UP", 1003},
			{"MOUSE_TILT_DOWN", 1004},
			{"MOUSE_SCROLL_UP", 1005},
			{"MOUSE_SCROLL_DOWN", 1006},
			{"STROKE_TURN_RIGHT", 1007},
			{"STROKE_TURN_LEFT", 1008},
			{"STROKE_TILT_UP", 1009},
			{"STROKE_TILT_DOWN", 1010},
			{"STROKE_ZOOM_IN", 1011},
			{"STROKE_ZOOM_OUT", 1012}
		};

		return keymap;
	}

	static int GetInputKeyFromString(const std::string& inputKeyString)
	{
		std::unordered_map<std::string, int> keymap = GetConfigurableKeys();

		auto it = keymap.find(inputKeyString);
		if(it != keymap.end())
		{
			return it->second;
		}
		else
		{
			return -1;
		}
	}

	static std::string Vec3ToString(const glm::vec3& vec3)
	{
		return std::to_string(vec3.x) + ", " + std::to_string(vec3.y) + ", " + std::to_string(vec3.z);
	}

	static std::string Vec4ToString(const glm::vec4& vec4)
	{
		return std::to_string(vec4.r) + ", " + std::to_string(vec4.g) + ", " + std::to_string(vec4.b) + ", " + std::to_string(vec4.a);
	}

	static const char* GetGLErrorString(GLenum error)
	{
		switch (error)
		{
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		default: return "UNKNOWN_ERROR";
		}
	}

	static bool OpenGLError()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("OpenGL Error: 0x%X (%s)\n", error, GetGLErrorString(error));
			return true;
		}

		return false;
	}

	static glm::mat4 CreateRenderPropertiesMat(float resolutionX, float resolutionY, float nearPlane, float farPlane, float cameraPitch)
	{
		glm::mat4 renderProperties;
		renderProperties[0][0] = resolutionX;
		renderProperties[0][1] = resolutionY;
		renderProperties[0][2] = nearPlane;
		renderProperties[0][3] = farPlane;
		renderProperties[1][0] = cameraPitch;

		return renderProperties;
	}

	static glm::mat4 CreateOrthoProjectionMat(float screenWidth, float screenHeight)
	{
		float aspectRatio = screenWidth / screenHeight;
		return glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -100.0f, 100.0f);
	}

	static glm::mat4 CreateOrthoPixelProjectionMat(float screenWidth, float screenHeight)
	{
		return glm::ortho(0.0f, screenWidth, 0.0f, screenHeight, -100.0f, 100.0f);
	}

	static bool IsMobile()
	{
#ifdef __EMSCRIPTEN__
		std::string result = emscripten_run_script_string
		(
			"(/Mobi|Android|Tablet|iPad|iPhone/.test(navigator.userAgent)).toString();"
		);
		return result == "true";
#endif
		return false;
	}

	static glm::u8vec2 PackID(const uint32_t objectId)
	{
		glm::u8vec2 packedColor;

		packedColor.r = (objectId >> 0) & 0xFF;
		packedColor.g = (objectId >> 8) & 0xFF;

		return packedColor;
	}

	static FAFPickID UnpackID(const GLubyte* pixel)
	{
		FAFPickID pickID = {};

		pickID.objectId = static_cast<uint32_t>(pixel[0]) | (static_cast<uint32_t>(pixel[1]) << 8);
		pickID.elementId = static_cast<uint8_t>(pixel[2]);

		return pickID;
	}

	// #Hack.
	// Curves should be a part of skeleton, but since I have only "one" I can have this easily accessible.
	static std::vector<std::string> GetCurveNames()
	{
		return
	 {
			"rootDistance",
			"rootYaw",
			"rootYawDelta",
			"rootYawAuthority",
			"rootYawWarpScale",
			"rootYawLeft"
		};
	}

	// DJB2-like.
	// @todo Learn about it.
	static constexpr unsigned int StringSwitch(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (StringSwitch(str, h + 1) * 33) ^ str[h];
	}

	static float GetAngleTowardsMovementInput();

	// This uses information from both character movement and anim state components.
	// Which means if we want to use this correctly, we have to call it in after both have updated, in like late tick of AnimState.
	static float GetRootAngleTowardsMovementInput();

	static float GetRootAngleTowardsVelocity();
	static float GetAngleTowardsVelocity();

	static glm::vec3 GetColorVector(EAFColor color);

	static void DrawDebugBox(const glm::vec3& location,
		float size = 1.0f,
		float lifetime = 3.0f,
		EAFColor color = EAFColor::Red,
		const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f));

	static void GetBone(const AFPose& pose, const std::string& bone, EAFBoneSpace space, glm::vec3& location, glm::vec3& rotation);

	// First bone is IK root, last bone is IK effector.
	// Target Location and Target Rotation are in global space (root bone is 000), so convert them first if they are in world space.
	static void CCDIK(AFPose& pose,
		const std::vector<FAFIKBoneProperties>& ikbones,
		const glm::vec3 targetLocation,
		const glm::quat targetRotation,
		float threshold = 0.001f,
		size_t maxIterations = 20);

private:

	static void DrawDebugActor(std::shared_ptr<class AFDebugShapeActor> actor,
		const glm::vec3& location,
		float lifetime = 3.0f,
		const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f),
		EAFColor color = EAFColor::Red);

};

namespace AFIDGenerator
{
	inline unsigned int Next()
	{
		static unsigned int counter = 0;
		return ++counter;
	}
}

#define DebugOpenGL() if(AFUtility::OpenGLError()) assert(false)
#define ClearOpenGLErrors() while (glGetError() != GL_NO_ERROR)
