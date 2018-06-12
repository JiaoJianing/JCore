#include "stdafx.h"
#include "SnowParticleSystemComponent.h"
#include "World.h"

SnowParticleSystemComponent::SnowParticleSystemComponent()
{
}


SnowParticleSystemComponent::~SnowParticleSystemComponent()
{
}

stringT SnowParticleSystemComponent::GetTypeName()
{
	return _T("SnowParticleSystemComponent");
}

void SnowParticleSystemComponent::Update(double curFrame, double deltaFrame)
{
	m_SnowParticleSys.SetIsActive(GetIsActive());
	m_SnowParticleSys.Update();
}

void SnowParticleSystemComponent::OnAddToWorld(World* world)
{
	world->GetScene()->GetParticleSystems().push_back(&m_SnowParticleSys);
}

void SnowParticleSystemComponent::OnRemoveFromWorld(World* world)
{
	auto it = std::find(world->GetScene()->GetParticleSystems().begin(), world->GetScene()->GetParticleSystems().end(), &m_SnowParticleSys);
	if (it != world->GetScene()->GetParticleSystems().end()) {
		world->GetScene()->GetParticleSystems().erase(it);
	}
}

void SnowParticleSystemComponent::Initialize(int particleCount)
{
	m_SnowParticleSys.Initialize(particleCount);
}
