#include "stdafx.h"
#include "Shader.h"

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	unsigned int vertex, fragment, geometry;
	vertex = compileVertexShader(vertexPath);
	fragment = compileFragmentShader(fragmentPath);
	geometry = compileGeometryShader(geometryPath);

	linkProgram(vertex, fragment, geometry);

	//ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ�������Ҫ
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

	//ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ�������Ҫ
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
	//������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &shaderCode, NULL);
	glCompileShader(vertex);
	//��ӡ�����������У�
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
	//Ƭ����ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &shaderCode, NULL);
	glCompileShader(fragment);
	//��ӡ�����������У�
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
		//������ɫ��
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &shaderCode, NULL);
		glCompileShader(geometry);
		//��ӡ�����������У�
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
	//��ɫ������
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
	//��ӡ���Ӵ�������У�
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "Link Shader Failed: " << infoLog << std::endl;
	}
}

std::string Shader::readShaderFile(const GLchar* path)
{
	//���ļ�·����ȡ������ɫ��
	std::string code;
	if (path != NULL) {
		std::ifstream shaderFile;
		//��֤ifstream��������׳��쳣
		shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//���ļ�
			shaderFile.open(path);
			std::stringstream shaderStream;
			//��ȡ�ļ����ݵ�������
			shaderStream << shaderFile.rdbuf();
			//�ر��ļ�������
			shaderFile.close();
			//ת����������string
			code = shaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Read Shader File Failed: " << path << e.what() << std::endl;
		}
	}

	return code;
}
