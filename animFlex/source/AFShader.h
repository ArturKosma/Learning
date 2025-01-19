#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AFShader
{
	friend class AFRenderer;

public:

	bool LoadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void Use();
	void Cleanup();

private:

	AFShader();
	~AFShader();

	GLuint ReadShader(const std::string& shaderFilename, GLuint shaderType);

	GLuint m_shaderProgram = 0;
};
