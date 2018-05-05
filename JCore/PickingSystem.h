#pragma once

struct PickInfo {	
	float nodeID;
	float reserved1;
	float reserved2;
};

class PickingSystem
{
public:
	PickingSystem(int windowWidth, int windowHeight);
	~PickingSystem();

	void BeginRender();

	void EndRender();

	PickInfo Pick(unsigned int x, unsigned int y);

public:
	unsigned int m_FBO;
	unsigned int m_PickTexture;
	unsigned int m_DepthTexture;
};

