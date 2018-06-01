#include "stdafx.h"
#include <iostream>
#include "stb_image.h"
#include "Model.h"
#include "ResourceManager.h"
#include "MatrixHelper.h"

Model::Model(const char* path)
	: m_Color(1.0f)
	, m_Scene(0)
	, m_NumBones(0)
	, m_SupportAnimation(false)
{
	loadModel(path);
}

Model::Model()
	: m_Color(1.0f)
	, m_Scene(0)
	, m_NumBones(0)
	, m_SupportAnimation(false)
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
	if (m_SupportAnimation) {
		for (unsigned i = 0; i < m_BoneTransforms.size(); i++) {
			shader.setMatrix4("gBones[" + std::to_string(i) + "]", m_BoneTransforms[i]);
		}
	}

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

bool Model::GetSupportAnimation()
{
	return m_SupportAnimation;
}

void Model::UpdateBoneTransform(float currentFrame)
{
	glm::mat4 identity;

	float TicksPerSecond = (float)(m_Scene->mAnimations[0]->mTicksPerSecond != 0 ? m_Scene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float TimeInTicks = currentFrame * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, (float)m_Scene->mAnimations[0]->mDuration);

	readNodeHeirarchy(AnimationTime, m_Scene->mRootNode, identity);

	for (unsigned i = 0; i < m_NumBones; i++) {
		m_BoneTransforms[i] = m_BoneInfo[i].FinalTransform;
	}
}

void Model::loadModel(std::string path)
{
	m_Scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | 
		aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices);

	if (!m_Scene || m_Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_Scene->mRootNode) {
		std::cout << "Assimp load failed: " << m_Importer.GetErrorString() << std::endl;
		return;
	}
	m_SupportAnimation = m_Scene->HasAnimations();

	directory = path.substr(0, path.find_last_of('/'));

	processNode(m_Scene->mRootNode, m_Scene);

	m_BoneTransforms.resize(m_NumBones);
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
	std::vector<VertexBone> bones;
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

	if (m_SupportAnimation) {
		bones.resize(vertices.size());
		processBones(mesh, bones);
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
		normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return Mesh(vertices, indices, bones, textures);
}

void Model::processBones(aiMesh* mesh, std::vector<VertexBone>& bones)
{
	for (unsigned int i = 0; i < mesh->mNumBones; i++) {
		unsigned int BoneIndex = 0;
		std::string BoneName(mesh->mBones[i]->mName.data);

		if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
			// Allocate an index for a new bone
			BoneIndex = m_NumBones;
			m_NumBones++;
			BoneInfo bi;
			m_BoneInfo.push_back(bi);
			m_BoneInfo[BoneIndex].BoneOffset = MatrixHelper::Convert2GlmMatrix(mesh->mBones[i]->mOffsetMatrix);
			m_BoneMapping[BoneName] = BoneIndex;
		}
		else {
			BoneIndex = m_BoneMapping[BoneName];
		}

		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {
			unsigned int VertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float Weight = mesh->mBones[i]->mWeights[j].mWeight;
			bones[VertexID].AddVertexBone(BoneIndex, Weight);
		}
	}
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

void Model::readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
{
	std::string NodeName(pNode->mName.data);

	const aiAnimation* pAnimation = m_Scene->mAnimations[0];

	glm::mat4 NodeTransformation(MatrixHelper::Convert2GlmMatrix(pNode->mTransformation));

	const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// Interpolate scaling and generate scaling transformation matrix
		aiVector3D Scaling;
		calcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		glm::mat4 ScalingM;
		ScalingM = glm::scale(ScalingM, glm::vec3(Scaling.x, Scaling.y, Scaling.z));

		// Interpolate rotation and generate rotation transformation matrix
		aiQuaternion RotationQ;
		calcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		glm::mat4 RotationM = MatrixHelper::Convert2GlmMatrix(RotationQ.GetMatrix());

		// Interpolate translation and generate translation transformation matrix
		aiVector3D Translation;
		calcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		glm::mat4 TranslationM;
		TranslationM = glm::translate(TranslationM, glm::vec3(Translation.x, Translation.y, Translation.z));

		// Combine the above transformations
		NodeTransformation = TranslationM * RotationM * ScalingM;
	}

	glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
		unsigned int BoneIndex = m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransform = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
		readNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
	}
}

void Model::calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1) {
		Out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int ScalingIndex = findScaling(AnimationTime, pNodeAnim);
	unsigned int NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
	float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

void Model::calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->mNumRotationKeys == 1) {
		Out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int RotationIndex = findRotation(AnimationTime, pNodeAnim);
	unsigned int NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
	float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
	Out = Out.Normalize();
}

void Model::calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1) {
		Out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int PositionIndex = findPosition(AnimationTime, pNodeAnim);
	unsigned int NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
	float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
	float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D Delta = End - Start;
	Out = Start + Factor * Delta;
}

unsigned int Model::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int Model::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

unsigned int Model::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
			return i;
		}
	}

	//assert(0);

	return 0;
}

const aiNodeAnim* Model::findNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
			return pNodeAnim;
		}
	}

	return 0;
}
