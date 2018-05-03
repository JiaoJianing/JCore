#include "stdafx.h"
#include "Game.h"
#include "ResourceManager.h"

Game::Game(unsigned int w, unsigned int h)
	: Width(w)
	, Height(h)
	, m_SpriteRenderer(0)
	, m_Particles(0)
	, m_Effects(0)
	, m_SoundEngine(0)
	, m_NormalText(0)
{
}


Game::~Game()
{
	if (m_SpriteRenderer != 0) {
		delete m_SpriteRenderer;
		m_SpriteRenderer = 0;
	}
	if (m_Particles != 0) {
		delete m_Particles;
		m_Particles = 0;
	}
	if (m_Effects != 0) {
		delete m_Effects;
		m_Effects = 0;
	}
	if (m_SoundEngine != 0) {
		m_SoundEngine->drop();
		m_SoundEngine = 0;
	}
	if (m_NormalText != 0) {
		delete m_NormalText;
		m_NormalText = 0;
	}
}

void Game::Init()
{
	ResourceManager::getInstance()->LoadShader("sprite", "asset/shaders/game/sprite.vs", "asset/shaders/game/sprite.fs");
	ResourceManager::getInstance()->LoadShader("particle", "asset/shaders/game/particle.vs", "asset/shaders/game/particle.fs");
	ResourceManager::getInstance()->LoadShader("effects", "asset/shaders/game/effects.vs", "asset/shaders/game/effects.fs");

	glm::mat4 projection = glm::ortho(0.0f, float(Width), float(Height), 0.0f, -1.0f, 1.0f);
	ResourceManager::getInstance()->GetShader("sprite").use().setInt("sprite", 0);
	ResourceManager::getInstance()->GetShader("sprite").setMatrix4("projection", projection);
	ResourceManager::getInstance()->GetShader("particle").use().setInt("particle", 0);
	ResourceManager::getInstance()->GetShader("particle").setMatrix4("projection", projection);

	//加载纹理
	ResourceManager::getInstance()->LoadTexture("background", "asset/resources/background.jpg");
	ResourceManager::getInstance()->LoadTexture("face", "asset/resources/awesomeface.png");
	ResourceManager::getInstance()->LoadTexture("block", "asset/resources/block.png");
	ResourceManager::getInstance()->LoadTexture("block_solid", "asset/resources/block_solid.png");
	ResourceManager::getInstance()->LoadTexture("paddle", "asset/resources/paddle.png");
	ResourceManager::getInstance()->LoadTexture("particle", "asset/resources/particle.png");

	m_SpriteRenderer = new SpriteRenderer(ResourceManager::getInstance()->GetShader("sprite"));
	
	m_Particles = new ParticleGenerator(ResourceManager::getInstance()->GetShader("particle"),
		ResourceManager::getInstance()->GetTexture("particle"), 500);

	m_Effects = new PostProcessor(ResourceManager::getInstance()->GetShader("effects"), this->Width, this->Height);

	m_SoundEngine = irrklang::createIrrKlangDevice();
	m_SoundEngine->play2D("asset/audios/breakout.mp3", true);

	m_NormalText = new Text(this->Width, this->Height);
	m_NormalText->Load("asset/fonts/HYShuYuanHeiJ.ttf", 36);
}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
		
	m_Effects->BeginRender();

	//绘制背景
	m_SpriteRenderer->DrawSprite(ResourceManager::getInstance()->GetTexture("background"), 
		glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);

	m_Effects->EndRender();

	m_Effects->Render(glfwGetTime());

	m_NormalText->Draw(L"Welcome to JCore", this->Width / 2 - 170, this->Height / 2, 1.0f, glm::vec3(0.6f, 0.7f, 0.8f));
}
