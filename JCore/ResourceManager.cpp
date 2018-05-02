#include "ResourceManager.h"

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
	Shader shader(vsPath.c_str(), fsPath.c_str());
	Shaders[name] = shader;

	return shader;
}

Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture ResourceManager::LoadTexture(std::string name, std::string path)
{
	Texture texture(path);
	Textures[name] = texture;

	return texture;
}

Texture ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
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
