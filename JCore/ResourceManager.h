#pragma once
#include "Shader.h"
#include "Texture.h"
#include <map>

class ResourceManager
{
public:
	~ResourceManager();

	static ResourceManager* getInstance();
	static void deleteInstance();

	std::map<std::string, Shader> Shaders;
	std::map<std::string, Texture> Textures;

	Shader LoadShader(std::string name, std::string vsPath, std::string fsPath, std::string gsPath);
	Shader LoadShader(std::string name, std::string vsPath, std::string fsPath);
	Shader GetShader(std::string name);
	Texture* LoadTexture(std::string name, std::string path);
	Texture* LoadTexture(const std::string& name,
		const std::string& dir,
		const std::string& posXFile,
		const std::string& negXFile,
		const std::string& posYFile,
		const std::string& negYFile,
		const std::string& posZFile,
		const std::string& negZFile);
	Texture* GetTexture(std::string name);
	void Clear();

private:
	ResourceManager();
	ResourceManager(const ResourceManager& text);
	ResourceManager& operator=(const ResourceManager& text);

private:
	static ResourceManager* m_Instance;
};

