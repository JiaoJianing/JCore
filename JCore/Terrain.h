#pragma once
#include <string>
#include <vector>
#include <glm.hpp>
#include "Shader.h"
#include "Texture.h"

class Terrain
{
public:
	Terrain(float heightScale, float blockScale);
	~Terrain();

	bool LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height);

	float GetHeightAt(const glm::vec3& position);

	void Render(Shader shader);

protected:
	void generateIndexBuffer();
	void generateNormals();
	void generateVertexBuffers();

private:
	int getFileLength(std::ifstream& file);
	float getHeightValue(const unsigned char* data, unsigned char numBytes);

private:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec3> m_Normals;
	std::vector<glm::vec2> m_TexCoords;
	std::vector<unsigned int> m_Indexs;

	glm::uvec2 m_HeightmapDimensions;
	float m_HeightScale;
	float m_BlockScale;

	unsigned int m_VAO, m_VBO, m_EBO;

	Texture m_GrassTexture;
	Texture m_RockTexture;
	Texture m_SnowTexture;
};

