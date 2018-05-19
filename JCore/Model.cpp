#include "stdafx.h"
#include <iostream>
#include "stb_image.h"
#include "Model.h"
#include "ResourceManager.h"

Model::Model(const char* path)
	: m_Color(1.0f)
{
	loadModel(path);
}

Model::Model()
	: m_Color(1.0f)
{

}

Model::~Model()
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].deleteGraphicsRes();
	}
}

void Model::LoadModel(const char* path)
{
	loadModel(path);
}

void Model::Render(Shader shader)
{
	shader.use();
	shader.setMatrix4("model", GetWorldTransform());
	shader.setInt("g_highLight", GetHighLight());
	shader.setVec3("g_highLightColor", GetHighLightColor());
	shader.setVec3("g_Color", GetColor());
	shader.setInt("nodeID", GetID());

	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shader);
	}
}

std::vector<Mesh>& Model::getMeshes()
{
	return meshes;
}

void Model::SetWorldTransform(const glm::mat4& mat)
{
	if (m_WorldTransform != mat) {
		m_WorldTransform = mat;
	}
}

glm::mat4& Model::GetWorldTransform()
{
	return m_WorldTransform;
}

void Model::SetHighLight(bool value)
{
	m_HighLight = value;
}

bool Model::GetHighLight()
{
	return m_HighLight;
}

void Model::SetHighLightColor(const glm::vec3& color)
{
	if (m_HighLightColor != color) {
		m_HighLightColor = color;
	}
}

glm::vec3& Model::GetHighLightColor()
{
	return m_HighLightColor;
}

void Model::SetID(unsigned long id)
{
	m_ID = id;
}

unsigned long Model::GetID()
{
	return m_ID;
}

glm::vec3& Model::GetColor()
{
	return m_Color;
}

void Model::SetColor(const glm::vec3& value)
{
	if (m_Color != value) {
		m_Color = value;
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp load failed: " << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//处理节点所有的网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//处理它的子节点
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		//位置
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.position = position;
		//法线
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;
		//uv坐标
		glm::vec2 texCoord;
		if (mesh->mTextureCoords[0])//检查是否有纹理
		{
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		vertex.texCoord = texCoord;
		//切线
		glm::vec3 tangent;
		if (mesh->mTangents) {
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
		}
		vertex.tangent = tangent;
		//副切线
		glm::vec3 bitangent;
		if (mesh->mBitangents) {
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;
		}
		vertex.bitangent = bitangent;

		vertices.push_back(vertex);
	}

	//处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//处理材质
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		std::vector<Texture> reflectMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_reflect");
		textures.insert(textures.end(), reflectMaps.begin(), reflectMaps.end());
		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		std::string fullPath = directory + "/" + str.C_Str();
		Texture* texture = ResourceManager::getInstance()->LoadTexture(fullPath, fullPath);
		texture->setType(typeName);
		textures.push_back(*texture);
	}

	return textures;
}