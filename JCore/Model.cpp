#include "stdafx.h"
#include <iostream>
#include "stb_image.h"
#include "Model.h"
#include "ResourceManager.h"

Model::Model(const char* path)
{
	loadModel(path);
}

Model::Model()
{

}

Model::~Model()
{
}

void Model::LoadModel(const char* path)
{
	loadModel(path);
}

void Model::Render(Shader shader)
{
	shader.use();
	shader.setMatrix4("model", GetWorldTransform());
	shader.setInt("highLight", GetHighLight());
	shader.setVec3("highLightColor", GetHighLightColor());
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

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp load failed: " << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//����ڵ����е�����
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	//���������ӽڵ�
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
		//λ��
		glm::vec3 position;
		position.x = mesh->mVertices[i].x;
		position.y = mesh->mVertices[i].y;
		position.z = mesh->mVertices[i].z;
		vertex.position = position;
		//����
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;
		//uv����
		glm::vec2 texCoord;
		if (mesh->mTextureCoords[0])//����Ƿ�������
		{
			texCoord.x = mesh->mTextureCoords[0][i].x;
			texCoord.y = mesh->mTextureCoords[0][i].y;
		}
		vertex.texCoord = texCoord;
		//����
		glm::vec3 tangent;
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		vertex.tangent = tangent;
		//������
		glm::vec3 bitangent;
		bitangent.x = mesh->mBitangents[i].x;
		bitangent.y = mesh->mBitangents[i].y;
		bitangent.z = mesh->mBitangents[i].z;
		vertex.bitangent = bitangent;

		vertices.push_back(vertex);
	}

	//��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	//�������
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
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].getPath().c_str(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			//�������û�м��ع����ͼ���
			std::string fullPath = directory + "/" + str.C_Str();
			Texture texture(fullPath);
			texture.setType(typeName);
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}