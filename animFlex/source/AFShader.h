#pragma once

#include <string>
#include <glad/glad.h>

class AFShader
{

public:

	AFShader();
	virtual ~AFShader();

	void SetVertexShader(const std::string& inPath);
	void SetFragmentShader(const std::string& inPath);

	void SetUniform1f(const GLchar* uniformName, float uniformFloat1);
	void SetUniform2f(const GLchar* uniformName, float uniformFloat1, float uniformFloat2);
	void SetUniform3f(const GLchar* uniformName, float uniformFloat1, float uniformFloat2, float uniformFloat3);

	bool LoadShaders();
	void Use() const;
	void Cleanup();

	GLuint GetProgram() const;

protected:

	GLuint ReadShader(const std::string& shaderFilename, GLuint shaderType);

	virtual void UniformBinding();

	std::string m_vertexShaderPath = {};
	std::string m_fragmentShaderPath = {};
	GLuint m_shaderProgram = 0;
};
