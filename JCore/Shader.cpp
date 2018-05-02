#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	unsigned int vertex, fragment, geometry;
	vertex = compileVertexShader(vertexPath);
	fragment = compileFragmentShader(fragmentPath);
	geometry = compileGeometryShader(geometryPath);

	linkProgram(vertex, fragment, geometry);

	//删除着色器，它们已经链接到我们的程序中了，不再需要
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

Shader::Shader()
	:ID(-1)
{
}

Shader::~Shader()
{
}

void Shader::load(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath /*= NULL*/)
{
	unsigned int vertex, fragment, geometry;
	vertex = compileVertexShader(vertexPath);
	fragment = compileFragmentShader(fragmentPath);
	geometry = compileGeometryShader(geometryPath);

	linkProgram(vertex, fragment, geometry);

	//删除着色器，它们已经链接到我们的程序中了，不再需要
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);
}

Shader& Shader::use()
{
	glUseProgram(ID);

	return *this;
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(const std::string& name, float x, float y)
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setFloat4(const std::string& name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value)
{
	setFloat2(name, value.x, value.y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value)
{
	setFloat3(name, value.x, value.y, value.z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value)
{
	setFloat4(name, value.x, value.y, value.z, value.w);
}

void Shader::setFloat3(const std::string& name, float x, float y, float z)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setMatrix4fv(const std::string& name, float* value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setMatrix3(const std::string& name, const glm::mat3& value)
{
	setMatrix3fv(name, (float*)glm::value_ptr(value));
}

void Shader::setMatrix4(const std::string& name, const glm::mat4& value)
{
	setMatrix4fv(name, (float*)glm::value_ptr(value));
}

void Shader::setMatrix3fv(const std::string& name, float* value)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

unsigned int Shader::compileVertexShader(const GLchar* vertexPath)
{
	unsigned int vertex = 0;
	std::string code = readShaderFile(vertexPath);
	const char* shaderCode = code.c_str();

	int success;
	char infoLog[512];
	//顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &shaderCode, NULL);
	glCompileShader(vertex);
	//打印编译错误（如果有）
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "Compile Vertex Shader " <<  vertexPath <<" Failed: " << infoLog << std::endl;
	}

	return vertex;
}

unsigned int Shader::compileFragmentShader(const GLchar* fragmentPath)
{
	unsigned int fragment = 0;	
	std::string code = readShaderFile(fragmentPath);
	const char* shaderCode = code.c_str();

	int success;
	char infoLog[512];
	//片段着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &shaderCode, NULL);
	glCompileShader(fragment);
	//打印编译错误（如果有）
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "Compile Fragment Shader " << fragmentPath << "  Failed: " << infoLog << std::endl;
	}

	return fragment;
}

unsigned int Shader::compileGeometryShader(const GLchar* geometryPath)
{
	unsigned int geometry = 0;
	if (geometryPath != NULL) {
		std::string code = readShaderFile(geometryPath);
		const char* shaderCode = code.c_str();

		int success;
		char infoLog[512];
		//几何着色器
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &shaderCode, NULL);
		glCompileShader(geometry);
		//打印编译错误（如果有）
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "Compile Geometry Shader " << geometryPath << "  Failed: " << infoLog << std::endl;
		}
	}

	return geometry;
}

void Shader::linkProgram(unsigned int vertex, unsigned int fragment, unsigned int geometry)
{
	//着色器程序
	ID = glCreateProgram();
	if (vertex != 0) {
		glAttachShader(ID, vertex);
	}
	if (fragment != 0) {
		glAttachShader(ID, fragment);
	}
	if (geometry != 0) {
		glAttachShader(ID, geometry);
	}
	glLinkProgram(ID);

	int success;
	char infoLog[512];
	//打印链接错误（如果有）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Link Shader Failed: " << infoLog << std::endl;
	}
}

std::string Shader::readShaderFile(const GLchar* path)
{
	//从文件路径获取几何着色器
	std::string code;
	if (path != NULL) {
		std::ifstream shaderFile;
		//保证ifstream对象可以抛出异常
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//打开文件
			shaderFile.open(path);
			std::stringstream shaderStream;
			//读取文件内容到数据流
			shaderStream << shaderFile.rdbuf();
			//关闭文件处理器
			shaderFile.close();
			//转换数据流到string
			code = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Read Shader File Failed: " << e.what() << std::endl;
		}
	}

	return code;
}
