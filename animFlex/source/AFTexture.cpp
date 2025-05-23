#include <stb/stb_image.h>

#include "AFTexture.h"

#include "AFUtility.h"

bool AFTexture::LoadTexture()
{
	int texWidth, texHeight, numberOfChannels;

	stbi_set_flip_vertically_on_load(m_verticallyFlipped);
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

	switch(numberOfChannels)
	{
		case 1:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, texWidth, texHeight, 0, GL_RED, GL_UNSIGNED_BYTE, textureData);
			break;
		}
		case 3:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
			break;
		}
		case 4:
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
			break;
		}
		default:
		{
			break;
		}
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(textureData);
	return true;
}

void AFTexture::Bind() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void AFTexture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AFTexture::Cleanup()
{
	glDeleteTextures(1, &m_texture);
}

bool AFTexture::LoadImpl(const char* filepath, bool boolean)
{
	m_verticallyFlipped = boolean;
	m_texturePath = filepath;

	return LoadTexture();
}
