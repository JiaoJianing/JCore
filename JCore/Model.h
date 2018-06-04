#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include <map>

struct BoneInfo {
	glm::mat4 BoneOffset;
	glm::mat4 FinalTransform;
};

//模型类
class Model
{
public:
	Model();
	Model(const char* path);
	~Model();

	void LoadModel(const char* path);

	void Render(Shader shader);

	std::vector<Mesh>& getMeshes();

	void SetWorldTransform(const glm::mat4& mat);
	glm::mat4& GetWorldTransform();

	void SetHighLight(bool value);
	bool GetHighLight();

	void SetHighLightColor(const glm::vec3& color);
	glm::vec3& GetHighLightColor();

	void SetID(unsigned long id);
	unsigned long GetID();

	glm::vec3& GetColor();
	void SetColor(const glm::vec3& value);

	bool GetSupportAnimation();

	void UpdateBoneTransform(float currentFrame);

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path);

	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	void processBones(aiMesh* mesh, std::vector<VertexBone>& bones);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	//骨骼动画相关
	void readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform);
	void calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	unsigned int findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	const aiNodeAnim* findNodeAnim(const std::string NodeName);

private:
	glm::vec3 m_Color;
	glm::mat4 m_WorldTransform;
	bool m_HighLight;
	glm::vec3 m_HighLightColor;
	unsigned long m_ID;
	bool m_SupportAnimation;

	std::map<std::string, unsigned int> m_BoneMapping; // maps a bone name to its index
	unsigned int m_NumBones;
	std::vector<BoneInfo> m_BoneInfo;
	glm::mat4 m_GlobalInverseTransform;
	std::vector<glm::mat4> m_BoneTransforms;

	std::map<std::string, aiNodeAnim*> m_AnimMaps;

	const aiScene* m_Scene;
	Assimp::Importer m_Importer;
};

