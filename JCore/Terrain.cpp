#include "stdafx.h"
#include "Terrain.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include "ResourceManager.h"

namespace fs = std::experimental::filesystem;

Terrain::Terrain(float heightScale, float blockScale)
	: m_HeightScale(heightScale)
	, m_BlockScale(blockScale)
{
	m_GrassTexture = *ResourceManager::getInstance()->LoadTexture("grass", "asset/resources/grass.jpg");
	m_RockTexture = *ResourceManager::getInstance()->LoadTexture("grass", "asset/resources/rock.jpg");
	m_SnowTexture = *ResourceManager::getInstance()->LoadTexture("grass", "asset/resources/snow.jpg");
}

Terrain::~Terrain()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

bool Terrain::LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height)
{
	if (!fs::exists(filename)) {
		std::cout << "Cant find file: " << filename << std::endl;
		return false;
	}

	std::ifstream ifs;
	ifs.open(filename, std::ifstream::binary);
	if (ifs.fail()) {
		std::cout << "Failed to open file: " << filename << std::endl;
		return false;
	}

	const unsigned int bytesPerPixel = bitsPerPixel / 8;
	const unsigned int expectedFileSize = (bytesPerPixel * width * height);
	const unsigned int fileSize = getFileLength(ifs);
	if (expectedFileSize != fileSize) {
		std::cout << "File Size Error: " << std::endl;
		return false;
	}

	unsigned char* heightMap = new unsigned char[fileSize];
	ifs.read((char*)heightMap, fileSize);
	if (ifs.fail()) {
		std::cout << "Error occurred when read height map file: " << filename << std::endl;
		ifs.close();
		delete[] heightMap;
		return false;
	}
	ifs.close();

	unsigned int numVerts = width * height;
	m_Positions.resize(numVerts);
	m_Normals.resize(numVerts);
	m_TexCoords.resize(numVerts);

	m_HeightmapDimensions = glm::uvec2(width, height);

	//世界单位
	float terrainWidth = (width - 1) * m_BlockScale;
	float terrainHeight = (height - 1) * m_BlockScale;

	float halfTerrainWidth = terrainWidth * 0.5f;
	float halfTerrainHeight = terrainHeight * 0.5f;

	for (unsigned int j = 0; j < height; j++) {
		for (unsigned int i = 0; i < width; i++) {
			unsigned int index = (j * width) + i;
			assert(index * bytesPerPixel < fileSize);
			float heightValue = getHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel);

			float S = (i / (float)(width - 1));
			float T = (j / (float)(height - 1));

			float X = (S * terrainWidth) - halfTerrainWidth;
			float Y = heightValue * m_HeightScale;
			float Z = (T * terrainHeight) - halfTerrainHeight;

			m_Normals[index] = glm::vec3(0.0f);
			m_Positions[index] = glm::vec3(X, Y, Z);
			m_TexCoords[index] = glm::vec2(S, T);
		}
	}

	std::cout << "Terrain loaded!" << std::endl;
	delete[] heightMap;

	generateIndexBuffer();
	generateNormals();
	generateVertexBuffers();

	return true;
}

float Terrain::GetHeightAt(const glm::vec3& position)
{
	float height = -FLT_MAX;
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2) {
		return height;
	}

	float terrainWidth = (m_HeightmapDimensions.x - 1) * m_BlockScale;
	float terrainHeight = (m_HeightmapDimensions.y - 1) * m_BlockScale;
	float halfWidth = terrainWidth * 0.5f;
	float halfHeight = terrainHeight * 0.5f;

	glm::vec3 terrainPos = position;
	glm::vec3 invBlockScale(1.0f / m_BlockScale, 0.0f, 1.0f / m_BlockScale);

	glm::vec3 offset(halfWidth, 0.0f, halfHeight);

	glm::vec3 vertexIndices = (terrainPos + offset) * invBlockScale;

	int u0 = (int)floorf(vertexIndices.x);
	int u1 = u0 + 1;
	int v0 = (int)floorf(vertexIndices.z);
	int v1 = v0 + 1;

	if (u0 >= 0 && u1 < (int)m_HeightmapDimensions.x && v0 >= 0 && v1 < (int)m_HeightmapDimensions.y) {
		glm::vec3 p00 = m_Positions[(v0 * m_HeightmapDimensions.x) + u0];
		glm::vec3 p10 = m_Positions[(v0 * m_HeightmapDimensions.x) + u1];
		glm::vec3 p01 = m_Positions[(v1 * m_HeightmapDimensions.x) + u0];
		glm::vec3 p11 = m_Positions[(v1 * m_HeightmapDimensions.x) + u1];

		float percentU = vertexIndices.x - u0;
		float percentV = vertexIndices.z - v0;

		glm::vec3 dU, dV;
		if (percentU > percentV) {
			dU = p10 - p00;
			dV = p11 - p10;
		}
		else {
			dU = p11 - p01;
			dV = p01 - p00;
		}

		glm::vec3 heightPos = p00 + (dU * percentU) + (dV * percentV);
		height = heightPos.y;
	}

	return height;
}

void Terrain::Render(Shader shader)
{
	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture.GetID());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_RockTexture.GetID());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_SnowTexture.GetID());

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_Indexs.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::generateIndexBuffer()
{
	if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2) {
		return;
	}

	const unsigned int terrainWidth = m_HeightmapDimensions.x;
	const unsigned int terrainHeight = m_HeightmapDimensions.y;

	const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;
	m_Indexs.resize(numTriangles * 3);

	unsigned int index = 0;
	for (unsigned int j = 0; j < (terrainHeight - 1); j++) {
		for (unsigned int i = 0; i < (terrainWidth - 1); i++) {
			int vertexIndex = (j * terrainWidth) + i;
			//TO
			m_Indexs[index++] = vertexIndex;
			m_Indexs[index++] = vertexIndex + terrainWidth + 1;
			m_Indexs[index++] = vertexIndex + 1;
			//T1
			m_Indexs[index++] = vertexIndex;
			m_Indexs[index++] = vertexIndex + terrainWidth;
			m_Indexs[index++] = vertexIndex + terrainWidth + 1;
		}
	}
}

void Terrain::generateNormals()
{
	for (unsigned int i = 0; i < m_Indexs.size(); i += 3) {
		glm::vec3 v0 = m_Positions[m_Indexs[i + 0]];
		glm::vec3 v1 = m_Positions[m_Indexs[i + 1]];
		glm::vec3 v2 = m_Positions[m_Indexs[i + 2]];

		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		m_Normals[m_Indexs[i + 0]] += normal;
		m_Normals[m_Indexs[i + 1]] += normal;
		m_Normals[m_Indexs[i + 2]] += normal;
	}

	const glm::vec3 up(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_Normals.size(); i++) {
		m_Normals[i] = glm::normalize(m_Normals[i]);
	}
}

void Terrain::generateVertexBuffers()
{
	unsigned int posSize = m_Positions.size() * sizeof(glm::vec3);
	unsigned int normalSize = m_Normals.size() * sizeof(glm::vec3);
	unsigned int uvSize = m_TexCoords.size() * sizeof(glm::vec2);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, posSize + normalSize + uvSize, 0, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, posSize, &m_Positions[0]);
	glBufferSubData(GL_ARRAY_BUFFER, posSize, normalSize, &m_Normals[0]);
	glBufferSubData(GL_ARRAY_BUFFER, posSize + normalSize, uvSize, &m_TexCoords[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)posSize);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)(posSize + normalSize));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indexs.size() * sizeof(unsigned int), &m_Indexs[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

int Terrain::getFileLength(std::ifstream& file)
{
	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(pos);

	return length;
}

float Terrain::getHeightValue(const unsigned char* data, unsigned char numBytes)
{
	switch (numBytes)
	{
	case 1:
	{
		return (unsigned char)(data[0]) / (float)0xff;
	}
	break;
	case 2:
	{
		return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
	}
	break;
	case 4:
	{
		return (unsigned int)(data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]) / (float)0xffffffff;
	}
	break;
	default:
		assert(false);
		break;
	}

	return 0.0;
}
