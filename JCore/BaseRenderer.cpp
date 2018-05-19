#include "stdafx.h"
#include "BaseRenderer.h"

BaseRenderer::BaseRenderer()
	: m_Width(0)
	, m_Height(0)
{
}


BaseRenderer::~BaseRenderer()
{
}

void BaseRenderer::Initialize(int width, int height)
{
	m_Width = width;
	m_Height = height;
}

void BaseRenderer::Resize(int width, int height)
{
	m_Width = width;
	m_Height = height;
}
