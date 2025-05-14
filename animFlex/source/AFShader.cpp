#include "AFShader.h"

#include <fstream>
#include <iostream>

void AFShader::SetUniform1f(const GLchar* uniformName, float uniformFloat1)
{
	Use();

	GLint resolutionUniformLoc = glGetUniformLocation(m_shaderProgram, uniformName);
	glUniform1f(resolutionUniformLoc, uniformFloat1);
}

void AFShader::SetUniform2f(const GLchar* uniformName, float uniformFloat1, float uniformFloat2)
{
	Use();

	GLint resolutionUniformLoc = glGetUniformLocation(m_shaderProgram, uniformName);
	glUniform2f(resolutionUniformLoc, uniformFloat1, uniformFloat2);
}

void AFShader::SetUniform3f(const GLchar* uniformName, float uniformFloat1, float uniformFloat2, float uniformFloat3)
{
	Use();

	GLint resolutionUniformLoc = glGetUniformLocation(m_shaderProgram, uniformName);
	glUniform3f(resolutionUniformLoc, uniformFloat1, uniformFloat2, uniformFloat3);
}

bool AFShader::LoadShaders()
{
	GLuint vertexShader = ReadShader(m_vertexShaderPath, GL_VERTEX_SHADER);
	if(!vertexShader)
	{
		printf("%s%s\n", "Error reading vertex shader, path: ", m_vertexShaderPath.c_str());
		return false;
	}

	GLuint fragmentShader = ReadShader(m_fragmentShaderPath, GL_FRAGMENT_SHADER);
	if(!fragmentShader)
	{
		printf("%s%s\n", "Error reading fragment shader, path: ", m_fragmentShaderPath.c_str());
		return false;
	}

	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, fragmentShader);
	glLinkProgram(m_shaderProgram);

	GLint isProgramLinked;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &isProgramLinked);
	if(!isProgramLinked)
	{
		return false;
	}

	UniformBinding();

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void AFShader::Use() const
{
	m_twoSidedMaterial ? glDisable(GL_CULL_FACE) : glEnable(GL_CULL_FACE);

	glUseProgram(m_shaderProgram);
}

void AFShader::Cleanup()
{
	glDeleteProgram(m_shaderProgram);
}

GLuint AFShader::GetProgram() const
{
	return m_shaderProgram;
}

bool AFShader::LoadImpl(const char* filepath1, const char* filepath2)
{
	m_vertexShaderPath = filepath1;
	m_fragmentShaderPath = filepath2;

	return LoadShaders();
}

AFShader::AFShader()
{
}

AFShader::~AFShader()
{
}

GLuint AFShader::ReadShader(const std::string& shaderFilename, GLuint shaderType)
{
	std::string shaderAsText;
	std::ifstream inFile(shaderFilename);

	if(inFile.is_open())
	{
		inFile.seekg(0, std::ios::end);
		shaderAsText.reserve(inFile.tellg());
		inFile.seekg(0, std::ios::beg);
		shaderAsText.assign(std::istreambuf_iterator<char>(inFile), std::istreambuf_iterator<char>());
		inFile.close();
	}
	else
	{
		return 0;
	}

	if(inFile.bad() || inFile.fail())
	{
		inFile.close();
		return 0;
	}

	inFile.close();

	const char* shaderSource = shaderAsText.c_str();
	GLuint shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, (const GLchar**)&shaderSource, 0);
	glCompileShader(shader);

	GLint isShaderCompiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
	if(!isShaderCompiled)
	{
		// Get the length of the log.
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// Retrieve the log.
		std::string infoLog(maxLength, '\0');
		glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

		// Print the log.
		printf("%s: %s\n%s\n", "Shader compilation failed", shaderFilename.c_str(), infoLog.c_str());

		// Clean up and return error
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

void AFShader::UniformBinding()
{
	glUseProgram(m_shaderProgram);

	GLint viewProjectionIndex = glGetUniformBlockIndex(m_shaderProgram, "ViewProjection");
	GLint inverseViewProjectionIndex = glGetUniformBlockIndex(m_shaderProgram, "InverseViewProjection");
	GLint modelIndex = glGetUniformBlockIndex(m_shaderProgram, "Model");
	GLint cameraIndex = glGetUniformBlockIndex(m_shaderProgram, "Camera");
	GLint renderPropertiesIndex = glGetUniformBlockIndex(m_shaderProgram, "RenderProperties");
	GLint uiTransformIndex = glGetUniformBlockIndex(m_shaderProgram, "UITransform");
	GLint viewRotationIndex = glGetUniformBlockIndex(m_shaderProgram, "ViewRotation");
	GLint orthoProjectionIndex = glGetUniformBlockIndex(m_shaderProgram, "OrthoProjection");
	GLint orthoPixelProjectionIndex = glGetUniformBlockIndex(m_shaderProgram, "OrthoPixelProjection");
	GLint jointMatricesIndex = glGetUniformBlockIndex(m_shaderProgram, "JointMatrices");

	if (viewProjectionIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, viewProjectionIndex, 0);
	}

	if (inverseViewProjectionIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, inverseViewProjectionIndex, 1);
	}

	if(modelIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, modelIndex, 2);
	}

	if(cameraIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, cameraIndex, 3);
	}

	if (renderPropertiesIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, renderPropertiesIndex, 4);
	}

	if (uiTransformIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, uiTransformIndex, 5);
	}

	if (viewRotationIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, viewRotationIndex, 6);
	}

	if (orthoProjectionIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, orthoProjectionIndex, 7);
	}

	if (orthoPixelProjectionIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, orthoPixelProjectionIndex, 8);
	}

	if (jointMatricesIndex != -1)
	{
		glUniformBlockBinding(m_shaderProgram, jointMatricesIndex, 9);
	}

	glUseProgram(0);
}
