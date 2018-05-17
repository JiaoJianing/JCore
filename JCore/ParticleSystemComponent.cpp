#include "stdafx.h"
#include "ParticleSystemComponent.h"

ParticleSystemComponent::ParticleSystemComponent()
{
}


ParticleSystemComponent::~ParticleSystemComponent()
{
}

stringT ParticleSystemComponent::GetTypeName()
{
	return _T("ParticleSystemComponent");
}

void ParticleSystemComponent::Update(double curFrame, double deltaFrame)
{
}

void ParticleSystemComponent::OnAddToWorld(World* world)
{
}

void ParticleSystemComponent::OnRemoveFromWorld(World* world)
{
}

void ParticleSystemComponent::Initialize(int particleCount)
{

}
