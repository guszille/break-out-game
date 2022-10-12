#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "core/Texture.h"

#include "util/SpriteRenderer.h"

class GameObject
{
public:
	Texture m_Sprite;

	glm::vec2 m_Position, m_Size, m_Velocity;
	glm::vec3 m_Color;

	float m_Rotation;

	bool m_Solid, m_Destroyed;

	GameObject();
	GameObject(Texture sprite, glm::vec2 position, glm::vec2 size, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

	~GameObject();

	virtual void draw(SpriteRenderer& renderer);
};
