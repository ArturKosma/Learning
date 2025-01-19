#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AFTexture
{
	friend class AFRenderer;

public:

	bool LoadTexture(std::string textureFilename);

	void Bind();
	void UnBind();

	void Cleanup();

private:

	AFTexture();
	~AFTexture();

	GLuint m_texture = 0;
};