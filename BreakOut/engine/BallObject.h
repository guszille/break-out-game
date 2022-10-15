#pragma once

#include <glm/glm.hpp>

#include "core/Texture.h"

#include "GameObject.h"

class BallObject : public GameObject
{
public:
	float m_Radius;

	bool m_Stuck;

	BallObject();
	BallObject(Texture sprite, glm::vec2 position, float radius, glm::vec2 velocity);

	~BallObject();

	glm::vec2 move(float deltaTime, unsigned int windowWidth);

	void reset(glm::vec2 position, glm::vec2 velocity);
};
