#include "AFShader.h"

#include <fstream>
#include <iostream>

bool AFShader::LoadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	GLuint vertexShader = ReadShader(vertexShaderFilename, GL_VERTEX_SHADER);
	if(!vertexShader)
	{
		printf("%s\n", "1");
		return false;
	}

	GLuint fragmentShader = ReadShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);
	if(!fragmentShader)
	{
		printf("%s\n", "2");
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return true;
}

void AFShader::Use()
{
	glUseProgram(m_shaderProgram);
}

void AFShader::Cleanup()
{
	glDeleteProgram(m_shaderProgram);
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
