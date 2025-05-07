#pragma once

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AFTexture
{
	friend class AFRenderer;

public:

	void SetTexture(const std::string& texturePath, bool verticallyFlipped = true);
	bool LoadTexture();

	void Bind() const;
	void UnBind() const;

	void Cleanup();

private:

	bool m_verticallyFlipped = true;
	std::string m_texturePath = {};
	GLuint m_texture = 0;
};