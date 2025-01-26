#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AFTexture
{
	friend class AFRenderer;

public:

	void SetTexture(const std::string& texturePath);
	bool LoadTexture();

	void Bind();
	void UnBind();

	void Cleanup();

private:

	std::string m_texturePath = {};
	GLuint m_texture = 0;
};