#pragma once
#include <string>

//贴图信息
class Texture {
public:
	Texture();
	Texture(const std::string& path);

	unsigned int GetID();
	void setID(unsigned int id);

	std::string getType();
	void setType(const std::string& type);

	std::string getPath();	
	void setPath(const std::string& path);

	static unsigned int loadTexture(const std::string& path);

	void Generate(unsigned int width, unsigned int height, unsigned char* data);

private:
	unsigned int m_ID;
	std::string m_type;
	std::string m_path;//记录已加载的纹理路径 防止重复加载
};