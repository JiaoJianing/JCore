#include "stdafx.h"
#include "Text.h"
#include "ResourceManager.h"

Text::Text(unsigned int width, unsigned int height)
{
	initGraphicsRes(width, height);
}

Text::~Text()
{
	deleteGraphicsRes();
	for (std::map<wchar_t, Character>::iterator it = m_Characters.begin(); it != m_Characters.end(); it++ ) {
		glDeleteTextures(1, &it->second.textureID);
	}
	FT_Done_Face(m_Face);
	FT_Done_FreeType(m_Ft);
}

void Text::Load(std::string font, unsigned int fontSize)
{
	if (FT_Init_FreeType(&m_Ft)) {
		std::cout << "Failed to init FreeType Library" << std::endl;
	}
	if (FT_New_Face(m_Ft, font.c_str(), 0, &m_Face)) {
		std::cout << "Failed to load font" << std::endl;
	}
	FT_Set_Pixel_Sizes(m_Face, 0, fontSize);
	FT_Select_Charmap(m_Face, FT_ENCODING_UNICODE);

	this->loadChar(L'H');
}

Character Text::loadChar(wchar_t ch)
{
	if (m_Characters.find(ch) == m_Characters.end()) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//�����ַ�
		FT_UInt  glyph_index = FT_Get_Char_Index(m_Face, ch);
		if (FT_Load_Glyph(m_Face, glyph_index, FT_LOAD_RENDER)) {
			std::cout << "Faild to load: " << ch << std::endl;
		}
		//��������
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_Face->glyph->bitmap.buffer);
		//��������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//�洢�ַ�
		Character character = {
			texture,
			glm::ivec2(m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows),
			glm::ivec2(m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top),
			m_Face->glyph->advance.x
		};
		m_Characters.insert(std::pair<wchar_t, Character>(ch, character));
		glBindTexture(GL_TEXTURE_2D, 0);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	return m_Characters[ch];
}

void Text::initGraphicsRes(int width, int height)
{
	glm::mat4 projection = glm::ortho(0.0f, float(width), float(height), 0.0f);
	this->m_TextShader = ResourceManager::getInstance()->GetShader("text");
	this->m_TextShader.use().setInt("text", 0);
	this->m_TextShader.setMatrix4("projection", projection);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Text::deleteGraphicsRes()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
}

void Text::Draw(const std::wstring& text, float x, float y, float scale, glm::vec3 color)
{
	this->m_TextShader.use();
	this->m_TextShader.setVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);
	std::wstring::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = loadChar(*c);

		float xpos = x + ch.bearing.x * scale;
		float ypos = y + (this->m_Characters[L'H'].bearing.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		//��Ļ�������Ͻ��ǣ�0,0�������������½��ǣ�0,0��
		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 },
			{ xpos, ypos, 0.0, 0.0 },

			{ xpos, ypos + h, 0.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 1.0 },
			{ xpos + w, ypos, 1.0, 0.0 },
		};
		//���ı����ϻ�����������
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		//����VBO
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//�����ı���
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
		x += (ch.advance >> 6) * scale;
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::Resize(int width, int height)
{
	deleteGraphicsRes();
	initGraphicsRes(width, height);
}
