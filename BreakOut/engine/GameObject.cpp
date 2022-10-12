#include "GameObject.h"

GameObject::GameObject()
	: m_Sprite(), m_Position(0.0f), m_Size(1.0f), m_Color(1.0f), m_Velocity(0.0f), m_Rotation(0.0f), m_Solid(false), m_Destroyed(false)
{
}

GameObject::GameObject(Texture sprite, glm::vec2 position, glm::vec2 size, glm::vec3 color, glm::vec2 velocity)
	: m_Sprite(sprite), m_Position(position), m_Size(size), m_Color(color), m_Velocity(velocity), m_Rotation(0.0f), m_Solid(false), m_Destroyed(false)
{
}

GameObject::~GameObject()
{
}

void GameObject::draw(SpriteRenderer& renderer)
{
	renderer.draw(m_Sprite, m_Position, m_Rotation, m_Size, m_Color);
}
