#pragma once
#include "BaseComponent.h"
#include "SRTTransformComponent.h"
#include <irrKlang.h>

class SoundComponent : public BaseComponent
{
public:
	SoundComponent();
	~SoundComponent();

	virtual stringT GetTypeName();

	virtual void Update(double curFrame, double deltaFrame);

	virtual void OnAddToWorld(World* world);

	virtual void OnRemoveFromWorld(World* world);

	void SetSound(std::string soundPath);

	void Play();

	void PlayOnce();
	
	void Pause();

	void Stop();

	void SetMinDistance(float dis);

	void SetPosition(const glm::vec3& pos);

	void SetPlaybackSpeed(float value);

	void SetLoop(bool value);

private:
	SRTTransformComponent * m_SrtCmp;
	irrklang::ISound* m_Sound;
	irrklang::ISound* m_LastSound;
};

