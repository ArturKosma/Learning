#pragma once

#include <string>
#include <glad/glad.h>

class AFShader
{

public:

	void SetVertexShader(const std::string& inPath);
	void SetFragmentShader(const std::string& inPath);

	bool LoadShaders();
	void Use();
	void Cleanup();

private:

	GLuint ReadShader(const std::string& shaderFilename, GLuint shaderType);

	std::string m_vertexShaderPath = {};
	std::string m_fragmentShaderPath = {};
	GLuint m_shaderProgram = 0;
};
