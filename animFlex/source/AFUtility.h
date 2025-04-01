#pragma once

#include <string>
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

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
			{"KEY_ESCAPE", GLFW_KEY_ESCAPE},
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
			{"STROKE_TILT_DOWN", 1010}
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
		return glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
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
};

#define DebugOpenGL() if(AFUtility::OpenGLError()) assert(false)
#define ClearOpenGLErrors() while (glGetError() != GL_NO_ERROR)
