#pragma once

#include <string>
#include <glad/glad.h>

#include "AFStructs.h"

class AFTexture : public FAFAsset
{
	friend class AFRenderer;

public:

	void Bind() const;
	void UnBind() const;

	void Cleanup();

protected:

	bool LoadImpl(const char* filepath, bool boolean) override;
	bool LoadTexture();

	bool m_verticallyFlipped = true;
	std::string m_texturePath = {};
	GLuint m_texture = 0;
};
