#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "AFTexture.h"

bool AFTexture::LoadTexture()
{
	int texWidth, texHeight, numberOfChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* textureData = stbi_load(m_texturePath.c_str(), &texWidth, &texHeight, &numberOfChannels, 0);

	if(!textureData)
	{
		stbi_image_free(textureData);
		return false;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
	return true;
}

void AFTexture::SetTexture(const std::string& texturePath)
{
	m_texturePath = texturePath;
}

void AFTexture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void AFTexture::UnBind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AFTexture::Cleanup()
{
	glDeleteTextures(1, &m_texture);
}
