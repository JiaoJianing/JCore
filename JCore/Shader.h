#pragma once
#include <glad/glad.h>
#include <glm.hpp>
#include <string>

class Shader
{
public:
	//程序ID
	unsigned int ID;

	//构造器读取并构建着色器
	Shader();
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = NULL);
	~Shader();

	void load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = NULL);

	//使用/激活程序
	Shader& use();

	//uniform 工具函数
	void setBool(const std::string& name, bool value) const;

	void setInt(const std::string& name, int value) const;

	void setFloat(const std::string& name, float value) const;

	void setFloat2(const std::string& name, float x, float y);

	void setFloat3(const std::string& name, float x, float y, float z);

	void setFloat4(const std::string& name, float x, float y, float z, float w);

	void setVec2(const std::string& name, const glm::vec2& value);

	void setVec3(const std::string& name, const glm::vec3& value);

	void setVec4(const std::string& name, const glm::vec4& value);

	void setMatrix3fv(const std::string& name, float* value);

	void setMatrix4fv(const std::string& name, float* value);

	void setMatrix3(const std::string& name, const glm::mat3& value);
	
	void setMatrix4(const std::string& name, const glm::mat4& value);

private:
	unsigned int compileVertexShader(const GLchar* vertexPath);
	unsigned int compileFragmentShader(const GLchar* fragmentPath);
	unsigned int compileGeometryShader(const GLchar* geometryPath);
	void linkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry);
	std::string readShaderFile(const GLchar* path);
};

