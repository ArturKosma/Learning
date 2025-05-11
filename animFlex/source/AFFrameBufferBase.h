#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "AFShader.h"
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

	virtual void DrawToScreen(const FAFSceneData& sceneData);
	void Cleanup();

	std::shared_ptr<AFShader> GetIDPickerShader(bool ui = false) const;

protected:

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_basicFramebuffer = 0;
	GLuint m_basicColorTex = 0;
	GLuint m_basicDepthBuffer = 0;

	std::shared_ptr<AFShader> m_idPickerShader = nullptr;
	std::shared_ptr<AFShader> m_idPickerUIShader = nullptr;

	virtual void Delete();

	AFFrameBufferBase() = default;
	virtual ~AFFrameBufferBase();
};
