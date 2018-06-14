#include "stdafx.h"
#include "SoundComponent.h"
#include "Node.h"
#include "ResourceManager.h"

SoundComponent::SoundComponent()
	: m_SrtCmp(0)
	, m_Sound(0)
{
}


SoundComponent::~SoundComponent()
{
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
