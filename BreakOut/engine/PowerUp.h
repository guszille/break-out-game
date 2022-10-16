#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "core/Texture.h"

#include "GameObject.h"

const glm::vec2 g_PowerUpSize(60.0f, 20.0f);
const glm::vec2 g_PowerUpVelocity(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
	std::string m_Type;
	float m_Duration;
	bool m_Activated;

	PowerUp(std::string type, float duration, Texture& texture, glm::vec2 position, glm::vec3 color);

	~PowerUp();
};
