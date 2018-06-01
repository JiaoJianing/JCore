#pragma once
#include "BaseComponent.h"
#include "SRTTransform.h"

class SRTTransformComponent :public  BaseComponent
{
public:
	SRTTransformComponent();
	~SRTTransformComponent();

	glm::vec3 GetScale();
	void SetScale(const glm::vec3& scale);

	//µ¥Î»£º½Ç¶È
	glm::vec3 GetRotate();
	void SetRotate(const glm::vec3& rotate);

	glm::vec3 GetTranslation();
	void SetTranslation(const glm::vec3& translation);

	bool GetCheckTerrain();
	void SetCheckTerrain(bool value);

	float GetHeightAboveTerrain();
	void SetHeightAboveTerrain(float value);

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

private:
	SRTTransform m_SRT;
	bool m_TransformDirty;
	World* m_World;
	bool m_CheckTerrain;
	float m_HeightAboveTerrain;
};

