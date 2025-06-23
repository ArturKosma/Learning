#pragma once

#include <string>
#include <glad/glad.h>

#include "AFAsset.h"

class AFTexture : public AFAsset
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
