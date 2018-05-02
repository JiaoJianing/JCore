#pragma once
#include <glm.hpp>
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"

struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float Life;

	Particle(): Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f){
	}
};

class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture texture, unsigned int amount);
	~ParticleGenerator();

	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f));

	void Draw();

private:
	void init();

	unsigned int firstUnusedParticle();

	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f));

private:
	std::vector<Particle> m_Particles;
	unsigned int m_Amount;
	Shader m_Shader;
	Texture m_Texture;
	unsigned int m_VAO, m_VBO;
	unsigned int m_LastUseParticle;
};

