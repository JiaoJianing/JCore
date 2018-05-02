#pragma once
#include <glm.hpp>
#include "Texture.h"
#include "SpriteRenderer.h"

class GameObject
{
public:
	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f));
	virtual ~GameObject();

	virtual void Draw(SpriteRenderer& renderer);
	
	glm::vec2 Position, Size, Velocity;
	glm::vec3 Color;
	float Rotation;
	bool IsSolid;
	bool Destroyed;
	Texture Sprite;

};

