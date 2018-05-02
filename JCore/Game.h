#pragma once
#include "SpriteRenderer.h"
#include "ParticleGenerator.h"
#include "PostProcessor.h"
#include "Text.h"
#include <irrKlang.h>
#include <tuple>

class Game
{
public:
	Game(unsigned int w, unsigned int h);
	~Game();

	bool Keys[1024];
	bool KeysProcessed[1024];
	unsigned int Width, Height;

	void Init();

	void ProcessInput(float dt);
	void Update(float dt);
	void Render();

private:
	SpriteRenderer * m_SpriteRenderer;
	ParticleGenerator* m_Particles;
	PostProcessor* m_Effects;

	irrklang::ISoundEngine* m_SoundEngine;

	Text* m_NormalText;
};

