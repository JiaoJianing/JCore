#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm.hpp>
#include <map>
#include "Shader.h"

struct Character {
	unsigned int textureID;//��������ID
	glm::ivec2 size;//���δ�С
	glm::ivec2 bearing;//�ӻ�׼�ߵ�������/������ƫ��ֵ
	unsigned int advance;//ԭ�������һ������ԭ��ľ���
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

