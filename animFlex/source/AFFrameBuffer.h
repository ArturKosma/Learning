#pragma once
#include <glad/glad.h>

class AFFramebuffer
{
	friend class AFRenderer;

public:

	bool Init(int width, int height);
	bool Resize(int newWidth, int newHeight);

	void Bind();
	void UnBind();

	void DrawToScreen();
	void Cleanup();

private:

	AFFramebuffer();
	~AFFramebuffer();

	bool CheckComplete() const;

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_buffer = 0;
	GLuint m_colorTex = 0;
	GLuint m_depthBuffer = 0;
};
