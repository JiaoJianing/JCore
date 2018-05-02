#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

//Ä£ÐÍÀà
class Model
{
public:
	Model();
	Model(const char* path);
	~Model();

	void LoadModel(const char* path);

	void Draw(Shader shader);

	std::vector<Mesh>& getMeshes();

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	std::vector<Texture> textures_loaded;
};

