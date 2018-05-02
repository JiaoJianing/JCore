#include "GameObject.h"



GameObject::GameObject()
	: Position(0.0f, 0.0f), Size(0.0f, 0.0f), Color(1.0f), Velocity(0.0f), Rotation(0.0f), IsSolid(false), Destroyed(false)
{
}


GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture sprite, glm::vec3 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f)*/)
	: Position(pos), Size(size), Sprite(sprite), Color(color), Velocity(velocity), Rotation(0.0f), IsSolid(false), Destroyed(false)
{

}

GameObject::~GameObject()
{
}

void GameObject::Draw(SpriteRenderer& renderer)
{
	renderer.DrawSprite(Sprite, Position, Size, Rotation, Color);
}
