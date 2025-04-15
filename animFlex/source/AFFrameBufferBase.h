#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "AFStructs.h"

class AFFrameBufferBase
{
	friend class AFRenderer;

public:

	virtual bool Init(int width, int height);

	virtual void Bind();
	virtual void UnBind();

	bool Resize(int newWidth, int newHeight);
	glm::vec2 GetSize() const;

	virtual void DrawToScreen(const AFSceneData& sceneData);
	void Cleanup();

	const AFShader& GetIDPickerShader() const;

protected:

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_basicFramebuffer = 0;
	GLuint m_basicColorTex = 0;
	GLuint m_basicDepthBuffer = 0;

	AFShader m_idPickerShader = AFShader();

	virtual void Delete();

	AFFrameBufferBase() = default;
	virtual ~AFFrameBufferBase();
};
