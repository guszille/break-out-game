#include "Game.h"

SpriteRenderer* g_Renderer;

GameObject* g_Player;

const glm::vec2 g_PlayerSize(100.0f, 20.0f);
const float g_PlayerVelocity(500.0f);

BallObject* g_Ball;

const glm::vec2 g_InitialBallVelocity(100.0f, -350.0f);
const float g_BallRadius = 12.5f;

ParticleGenerator* g_ParticleGenerator;

Game::Game(unsigned int width, unsigned int height)
	: m_ScreenWidth(width), m_ScreenHeight(height), m_KeysPressed(), m_State(GameState::ACTIVE), m_CurrentLevel(0)
{
}

Game::~Game()
{
	delete g_Renderer;
}

void Game::setup()
{
	glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_ScreenWidth, (float)m_ScreenHeight, 0.0f, -1.0f, 1.0f);

	ShaderProgram spriteRenderSP("shaders/sprite_render_vs.glsl", "shaders/sprite_render_fs.glsl");
	ShaderProgram particleRenderSP("shaders/particle_render_vs.glsl", "shaders/particle_render_fs.glsl");

	spriteRenderSP.bind();
	spriteRenderSP.setUniformMatrix4fv("uProjectionMatrix", projectionMatrix);
	spriteRenderSP.setUniform1i("uTexture", 0);
	spriteRenderSP.unbind();

	particleRenderSP.bind();
	particleRenderSP.setUniformMatrix4fv("uProjectionMatrix", projectionMatrix);
	particleRenderSP.setUniform1i("uTexture", 0);
	particleRenderSP.unbind();

	ResourceManager::setShaderProgram("particleRenderSP", particleRenderSP);
	ResourceManager::setShaderProgram("particleRenderSP", particleRenderSP);

	Texture awesomefaceTex("textures/awesomeface.png");
	Texture backgroundTex("textures/background.jpg");
	Texture solidBlockTex("textures/solid_block.png");
	Texture blockTex("textures/block.png");
	Texture paddleTex("textures/paddle.png");
	Texture particleTex("textures/particle.png");

	ResourceManager::setTexture("awesomefaceTex", awesomefaceTex);
	ResourceManager::setTexture("backgroundTex", backgroundTex);
	ResourceManager::setTexture("solidBlock", solidBlockTex);
	ResourceManager::setTexture("destructibleBlock", blockTex);
	ResourceManager::setTexture("paddleTex", paddleTex);
	ResourceManager::setTexture("particleTex", particleTex);

	GameLevel levelOne;
	GameLevel levelTwo;
	GameLevel levelThree;
	GameLevel levelFour;

	levelOne.load("levels/1.lvl", m_ScreenWidth, m_ScreenHeight / 2);
	levelTwo.load("levels/2.lvl", m_ScreenWidth, m_ScreenHeight / 2);
	levelThree.load("levels/3.lvl", m_ScreenWidth, m_ScreenHeight / 2);
	levelFour.load("levels/4.lvl", m_ScreenWidth, m_ScreenHeight / 2);

	m_Levels.push_back(levelOne);
	m_Levels.push_back(levelTwo);
	m_Levels.push_back(levelThree);
	m_Levels.push_back(levelFour);

	m_CurrentLevel = 0;

	glm::vec2 playerPosition(m_ScreenWidth / 2.0f - g_PlayerSize.x / 2.0f, m_ScreenHeight - g_PlayerSize.y);
	glm::vec2 ballPosition = playerPosition + glm::vec2(g_PlayerSize.x / 2.0f - g_BallRadius, -g_BallRadius * 2.0f);

	g_Player = new GameObject(paddleTex, playerPosition, g_PlayerSize);
	g_Ball = new BallObject(awesomefaceTex, ballPosition, g_BallRadius, g_InitialBallVelocity);

	g_Renderer = new SpriteRenderer(spriteRenderSP);

	g_ParticleGenerator = new ParticleGenerator(particleRenderSP, paddleTex, 500);
}

void Game::processInput(float deltaTime)
{
	if (m_State == GameState::ACTIVE)
	{
		float velocity = g_PlayerVelocity * deltaTime;

		if (m_KeysPressed[GLFW_KEY_A])
		{
			if (g_Player->m_Position.x >= 0.0f)
			{
				g_Player->m_Position.x -= velocity;

				if (g_Ball->m_Stuck)
				{
					g_Ball->m_Position.x -= velocity;
				}
			}
		}

		if (m_KeysPressed[GLFW_KEY_D])
		{
			if (g_Player->m_Position.x <= m_ScreenWidth - g_Player->m_Size.x)
			{
				g_Player->m_Position.x += velocity;

				if (g_Ball->m_Stuck)
				{
					g_Ball->m_Position.x += velocity;
				}
			}
		}

		if (m_KeysPressed[GLFW_KEY_SPACE])
		{
			g_Ball->m_Stuck = false;
		}
	}
}

void Game::update(float deltaTime)
{
	// Update ball movement.
	g_Ball->move(deltaTime, m_ScreenWidth);

	// Do ball collision.
	doCollision();

	if (g_Ball->m_Position.y >= m_ScreenHeight)
	{
		resetLevel();
		resetPlayer();
	}

	// Update particles.
	g_ParticleGenerator->update(deltaTime, *g_Ball, 2, glm::vec2(g_Ball->m_Radius / 2.0f));
}

void Game::render()
{
	if (m_State == GameState::ACTIVE)
	{
		Texture backgroundTex = ResourceManager::getTexture("backgroundTex");

		g_Renderer->draw(backgroundTex, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(m_ScreenWidth, m_ScreenHeight));

		m_Levels[m_CurrentLevel].draw(*g_Renderer);

		g_Player->draw(*g_Renderer);

		g_ParticleGenerator->draw(); // WARNING: Render particles before the ball.

		g_Ball->draw(*g_Renderer);
	}
}

void Game::setKeyState(int index, bool pressed)
{
	m_KeysPressed[index] = pressed;
}

void Game::doCollision()
{
	// Check collisions with the bricks.
	for (GameObject& brick : m_Levels[m_CurrentLevel].m_Bricks)
	{
		if (!brick.m_Destroyed)
		{
			Collision coll = checkCollision(*g_Ball, brick);

			if (std::get<0>(coll))
			{
				if (!brick.m_Solid)
				{
					brick.m_Destroyed = true;
				}

				Direction direction = std::get<1>(coll);
				glm::vec2 differece = std::get<2>(coll);

				if (direction == Direction::LEFT || direction == Direction::RIGHT) // Horizontal collision.
				{
					g_Ball->m_Velocity.x = -g_Ball->m_Velocity.x;

					float penetration = g_Ball->m_Radius - std::abs(differece.x);

					if (direction == Direction::LEFT)
					{
						g_Ball->m_Position.x += penetration;
					}
					else
					{
						g_Ball->m_Position.x -= penetration;
					}
				}
				else // Vertical collision.
				{
					g_Ball->m_Velocity.y = -g_Ball->m_Velocity.y;

					float penetration = g_Ball->m_Radius - std::abs(differece.y);

					if (direction == Direction::UP)
					{
						g_Ball->m_Position.y -= penetration;
					}
					else
					{
						g_Ball->m_Position.y += penetration;
					}
				}
			}
		}
	}

	// Check collision with the paddle.
	Collision coll = checkCollision(*g_Ball, *g_Player);

	if (!g_Ball->m_Stuck && std::get<0>(coll))
	{
		float boardCenter = g_Player->m_Position.x + g_Player->m_Size.x / 2.0f;
		float distanceFromCenter = (g_Ball->m_Position.x + g_Ball->m_Radius) - boardCenter;
		float percentage = distanceFromCenter / (g_Player->m_Size.x / 2.0f);
		float strength = 2.0f;

		glm::vec2 currentVelocity = g_Ball->m_Velocity;
		glm::vec2 newVelocity(g_InitialBallVelocity.x * percentage * strength, -1.0f * abs(g_Ball->m_Velocity.y));

		g_Ball->m_Velocity = glm::normalize(newVelocity) * glm::length(currentVelocity);
	}
}

void Game::resetLevel()
{
	switch (m_CurrentLevel)
	{
	case 0:
		m_Levels[0].load("levels/1.lvl", m_ScreenWidth, m_ScreenHeight / 2);
		break;

	case 1:
		m_Levels[1].load("levels/2.lvl", m_ScreenWidth, m_ScreenHeight / 2);
		break;

	case 2:
		m_Levels[2].load("levels/3.lvl", m_ScreenWidth, m_ScreenHeight / 2);
		break;

	case 3:
		m_Levels[3].load("levels/4.lvl", m_ScreenWidth, m_ScreenHeight / 2);
		break;
	}
}

void Game::resetPlayer()
{
	g_Player->m_Size = g_PlayerSize;
	g_Player->m_Position = glm::vec2(m_ScreenWidth / 2.0f - g_PlayerSize.x / 2.0f, m_ScreenHeight - g_PlayerSize.y);
	
	g_Ball->reset(g_Player->m_Position + glm::vec2(g_PlayerSize.x / 2.0f - g_BallRadius, -(g_BallRadius * 2.0f)), g_InitialBallVelocity);
}

Collision Game::checkCollision(BallObject& ball, GameObject& aabb)
{
	glm::vec2 ballCenter(ball.m_Position + ball.m_Radius);

	glm::vec2 aabbHalfExtents(aabb.m_Size.x / 2.0f, aabb.m_Size.y / 2.0f);
	glm::vec2 aabbCenter(aabb.m_Position.x + aabbHalfExtents.x, aabb.m_Position.y + aabbHalfExtents.y);

	glm::vec2 ballCenter2aabbCenter = ballCenter - aabbCenter;
	glm::vec2 clamped = glm::clamp(ballCenter2aabbCenter, -aabbHalfExtents, aabbHalfExtents);

	glm::vec2 aabbClosest = aabbCenter + clamped;
	glm::vec2 ballCenter2aabbClosest = aabbClosest - ballCenter;

	if (glm::length(ballCenter2aabbClosest) <= ball.m_Radius)
	{
		return std::make_tuple(true, getCollisionDirection(ballCenter2aabbClosest), ballCenter2aabbClosest);
	}
	else
	{
		return std::make_tuple(false, Direction::UP, glm::vec2(0.0f, 0.0f));
	}
}

Direction Game::getCollisionDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2( 0.0f,  1.0f), // UP
		glm::vec2( 1.0f,  0.0f), // RIGHT
		glm::vec2( 0.0f, -1.0f), // DOWN
		glm::vec2(-1.0f,  0.0f)	 // LEFT
	};

	float max = 0.0f;
	unsigned int bestMatch = -1;

	for (unsigned int i = 0; i < 4; i++)
	{
		float dot = glm::dot(glm::normalize(target), compass[i]);

		if (dot > max)
		{
			max = dot;
			bestMatch = i;
		}
	}

	return (Direction)bestMatch;
}
