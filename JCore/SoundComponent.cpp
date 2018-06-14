#include "stdafx.h"
#include "SoundComponent.h"
#include "Node.h"
#include "ResourceManager.h"

SoundComponent::SoundComponent()
	: m_SrtCmp(0)
	, m_Sound(0)
	, m_LastSound(0)
{
}


SoundComponent::~SoundComponent()
{
	if (m_LastSound) {
		m_LastSound->drop();
	}
}

stringT SoundComponent::GetTypeName()
{
	return _T("SoundComponent");
}

void SoundComponent::Update(double curFrame, double deltaFrame)
{
	if (m_SrtCmp == 0) {
		m_SrtCmp = GetOwner()->FindComponent<SRTTransformComponent>();
	}
	if (m_SrtCmp != 0) {
		glm::vec3 position = m_SrtCmp->GetTranslation();
		if (m_Sound) {
			m_Sound->setPosition(irrklang::vec3df(position.x, position.y, position.z));
		}
	}
}

void SoundComponent::OnAddToWorld(World* world)
{

}

void SoundComponent::OnRemoveFromWorld(World* world)
{

}

void SoundComponent::SetSound(std::string soundPath)
{
	m_Sound = ResourceManager::getInstance()->LoadSound3d(soundPath, soundPath);
}

void SoundComponent::Play()
{
	if (m_Sound) {
		m_Sound->setIsPaused(false);
	}
}

void SoundComponent::PlayOnce()
{
	if (m_Sound) {
		if (m_LastSound) {
			m_LastSound->drop();
		}
		m_LastSound = ResourceManager::getInstance()->GetSoundEngine()->play3D(m_Sound->getSoundSource(), m_Sound->getPosition());
	}
}

void SoundComponent::Pause()
{
	if (m_Sound) {
		m_Sound->setIsPaused(true);
	}
}

void SoundComponent::Stop()
{
	if (m_Sound) {
		m_Sound->stop();
	}
}

void SoundComponent::SetMinDistance(float dis)
{
	if (m_Sound) {
		m_Sound->setMinDistance(dis);
	}
}

void SoundComponent::SetPosition(const glm::vec3& pos)
{
	if (m_Sound) {
		m_Sound->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));
	}
}

void SoundComponent::SetPlaybackSpeed(float value)
{
	if (m_Sound) {
		m_Sound->setPlaybackSpeed(value);
	}
}

void SoundComponent::SetLoop(bool value)
{
	if (m_Sound) {
		m_Sound->setIsLooped(value);
	}
}
