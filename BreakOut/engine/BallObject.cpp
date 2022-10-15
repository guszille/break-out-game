#include "BallObject.h"

BallObject::BallObject()
	: GameObject(), m_Radius(12.5f), m_Stuck(true)
{
}

BallObject::BallObject(Texture sprite, glm::vec2 position, float radius, glm::vec2 velocity)
	: GameObject(sprite, position, glm::vec2(radius * 2.0f, radius * 2.0f), glm::vec3(1.0f), velocity), m_Radius(radius), m_Stuck(true)
{
}

BallObject::~BallObject()
{
}

glm::vec2 BallObject::move(float deltaTime, unsigned int windowWidth)
{
	if (!m_Stuck)
	{
        m_Position += m_Velocity * deltaTime;

        if (m_Position.x <= 0.0f)
        {
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = 0.0f;
        }
        else if (m_Position.x + m_Size.x >= windowWidth)
        {
            m_Velocity.x = -m_Velocity.x;
            m_Position.x = windowWidth - m_Size.x;
        }

        if (m_Position.y <= 0.0f)
        {
            m_Velocity.y = -m_Velocity.y;
            m_Position.y = 0.0f;
        }
	}

	return glm::vec2();
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
	m_Position = position;
	m_Velocity = velocity;
	m_Stuck = true;
}
