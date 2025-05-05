#pragma once

#include <map>
#include "AFStructs.h"

class AFTextRender
{
	friend class AFRenderer;

public:

	static AFTextRender& GetInstance();
	const std::map<char, FAFGlyph>& GetCharacters() const;

private:

	bool Init();

	std::map<char, FAFGlyph> m_characters;
};
