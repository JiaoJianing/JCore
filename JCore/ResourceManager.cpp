#include "stdafx.h"
#include "ResourceManager.h"
#include "CubeMapTexture.h"

ResourceManager::ResourceManager()
{
}


ResourceManager::ResourceManager(const ResourceManager& text)
{
}

ResourceManager* ResourceManager::m_Instance;

ResourceManager& ResourceManager::operator=(const ResourceManager& text)
{
	return *this;
}

ResourceManager::~ResourceManager()
{
}

ResourceManager* ResourceManager::getInstance()
{
	if (m_Instance == 0) {
		m_Instance = new ResourceManager();
	}

	return m_Instance;
}

void ResourceManager::deleteInstance()
{
	if (m_Instance != 0) {
		delete m_Instance;
		m_Instance = 0;
	}
}

Shader ResourceManager::LoadShader(std::string name, std::string vsPath, std::string fsPath, std::string gsPath)
{
	Shader shader(vsPath.c_str(), fsPath.c_str(), gsPath.c_str());
	Shaders[name] = shader;

	return shader;
}

Shader ResourceManager::LoadShader(std::string name, std::string vsPath, std::string fsPath)
{
	if (Shaders.find(name) != Shaders.end()) {
		return Shaders[name];
	}

	Shader shader(vsPath.c_str(), fsPath.c_str());
	Shaders[name] = shader;

	return shader;
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture* ResourceManager::LoadTexture(std::string name, std::string path)
{
	if (Textures.find(name) != Textures.end()) {
		std::cout << "Warning: load same texture name: " << name << std::endl;
		return &Textures[name];
	}

	Texture texture(path);
	Textures[name] = texture;

	return &Textures[name];
}

Texture* ResourceManager::LoadTexture(const std::string& name,
	const std::string& dir, 
	const std::string& posXFile, 
	const std::string& negXFile, 
	const std::string& posYFile, 
	const std::string& negYFile, 
	const std::string& posZFile, 
	const std::string& negZFile)
{
	if (Textures.find(name) != Textures.end()) {
		return &Textures[name];
	}

	CubeMapTexture texture(dir, posXFile, negXFile, posYFile, negYFile, posZFile, negZFile);
	Textures[name] = texture;

	return &Textures[name];
}

Texture* ResourceManager::GetTexture(std::string name)
{
	return &Textures[name];
}

void ResourceManager::Clear()
{
	for (auto iter : Shaders) {
		glDeleteProgram(iter.second.ID);
	}

	for (auto iter : Textures) {
		unsigned int id = iter.second.GetID();
		glDeleteTextures(1, &id);
	}

	Shaders.clear();
	Textures.clear();
}
