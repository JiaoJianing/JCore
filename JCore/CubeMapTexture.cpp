#include "stdafx.h"
#include "CubeMapTexture.h"
#include "stb_image.h"

CubeMapTexture::CubeMapTexture(const std::string& dir, 
	const std::string& posXFile, 
	const std::string& negXFile, 
	const std::string& posYFile, 
	const std::string& negYFile, 
	const std::string& posZFile, 
	const std::string& negZFile)
{
	m_ID = loadTexture(dir, posXFile, negXFile, posYFile, negYFile, posZFile, negZFile);
	m_path = dir;
}


CubeMapTexture::~CubeMapTexture()
{

}

unsigned int CubeMapTexture::loadTexture(const std::string& dir, 
	const std::string& posXFile, 
	const std::string& negXFile, 
	const std::string& posYFile, 
	const std::string& negYFile, 
	const std::string& posZFile, 
	const std::string& negZFile)
{
	std::vector<std::string> faces;
	faces.push_back(posXFile);
	faces.push_back(negXFile);
	faces.push_back(posYFile);
	faces.push_back(negYFile);
	faces.push_back(posZFile);
	faces.push_back(negZFile);
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (int i = 0; i < faces.size(); i++) {
		int width, height, nrComponents;
		unsigned char* data = stbi_load((dir + "/" + faces[i]).c_str(), &width, &height, &nrComponents, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Failed to load texture when create cube map texture: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
