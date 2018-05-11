#pragma once
#include "Texture.h"

class CubeMapTexture : public Texture
{
public:
	CubeMapTexture(const std::string& dir, 
		const std::string& posXFile,
		const std::string& negXFile,
		const std::string& posYFile,
		const std::string& negYFile,
		const std::string& posZFile,
		const std::string& negZFile);
	~CubeMapTexture();

	static unsigned int loadTexture(const std::string& dir,
		const std::string& posXFile,
		const std::string& negXFile,
		const std::string& posYFile,
		const std::string& negYFile,
		const std::string& posZFile,
		const std::string& negZFile);

private:

};

