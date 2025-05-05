#include "AFTextRender.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

AFTextRender& AFTextRender::GetInstance()
{
	static AFTextRender textRender;
	return textRender;
}

const std::map<char, FAFGlyph>& AFTextRender::GetCharacters() const
{
	return m_characters;
}

bool AFTextRender::Init()
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		printf("%s\n", "Could not init FreeType Library.");
		return false;
	}

	FT_Face face;
	if (FT_New_Face(ft, "content/fonts/arial.ttf", 0, &face))
	{
		printf("%s\n", "Failed to load font.");
		return false;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	// Required for any possible width of glyph textures.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load all first character glyphs textures and store them for later use.
	for(unsigned char c = 0; c < 128; ++c)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			printf("%s\n", "Failed to load glyph.");
			continue;
		}

		const unsigned int glyphWidth = face->glyph->bitmap.width;
		const unsigned int glyphHeight = face->glyph->bitmap.rows;

		// Generate texture.
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R8,
			glyphWidth,
			glyphHeight,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Texture options.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store for later use.
		FAFGlyph glyph =
		{
			texture,
			glm::ivec2(glyphWidth, glyphHeight),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<unsigned int>(face->glyph->advance.x)
		};
		m_characters.insert(std::pair<char, FAFGlyph>(c, glyph));
	}

	// Clear.
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return true;
}
