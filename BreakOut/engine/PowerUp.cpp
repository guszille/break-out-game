#include "PowerUp.h"

PowerUp::PowerUp(std::string type, float duration, Texture& texture, glm::vec2 position, glm::vec3 color)
	: GameObject(texture, position, g_PowerUpSize, color, g_PowerUpVelocity), m_Type(type), m_Duration(duration), m_Activated()
{
}

PowerUp::~PowerUp()
{
}
