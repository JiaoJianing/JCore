#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm.hpp>
#include <map>
#include "Shader.h"

struct Character {
	unsigned int textureID;//字形纹理ID
	glm::ivec2 size;//字形大小
	glm::ivec2 bearing;//从基准线到字形左部/顶部的偏移值
	unsigned int advance;//原点距离下一个字形原点的距离
};

class TextRenderer
{
public:
	TextRenderer(unsigned int width, unsigned int height);
	~TextRenderer();

	void Load(std::string font, unsigned int fontSize);

	void Draw(const std::wstring& text, float x, float y, float scale, glm::vec3 color);

	void Resize(int width, int height);

private:
	Character loadChar(wchar_t ch);

	void initGraphicsRes(int width, int height);
	void deleteGraphicsRes();

private:
	FT_Library m_Ft;
	FT_Face m_Face;
	std::map<wchar_t, Character> m_Characters;
	unsigned int m_VAO, m_VBO;
	Shader m_TextShader;
};

