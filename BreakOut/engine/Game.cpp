#include "Game.h"

SpriteRenderer* g_Renderer;
GameObject* g_Player;
BallObject* g_Ball;
ParticleGenerator* g_ParticleGenerator;
PostProcessor* g_Effects;
TextRenderer* g_TextRenderer;

const glm::vec2 g_PlayerSize(100.0f, 20.0f);
const float g_PlayerVelocity(500.0f);
const glm::vec2 g_InitialBallVelocity(100.0f, -350.0f);
const float g_BallRadius = 12.5f;

float g_ShakeTime = 0.0f;

irrklang::ISoundEngine* g_SoundEngine = irrklang::createIrrKlangDevice();

Game::Game(unsigned int width, unsigned int height)
	: m_ScreenWidth(width), m_ScreenHeight(height), m_KeysPressed(), m_KeysProcessed(), m_State(GameState::ON_MENU), m_CurrentLevel(0), m_Lives(3)
{
}

Game::~Game()
{
	delete g_Renderer;
	delete g_Player;
	delete g_Ball;
	delete g_ParticleGenerator;
	delete g_Effects;
	delete g_TextRenderer;
}

void Game::setup()
{
	glm::mat4 projectionMatrix = glm::ortho(0.0f, (float)m_ScreenWidth, (float)m_ScreenHeight, 0.0f, -1.0f, 1.0f);

	ShaderProgram spriteRenderSP("shaders/sprite_render_vs.glsl", "shaders/sprite_render_fs.glsl");
	ShaderProgram particleRenderSP("shaders/particle_render_vs.glsl", "shaders/particle_render_fs.glsl");
	ShaderProgram effectsRenderSP("shaders/postprocess_render_vs.glsl", "shaders/postprocess_render_fs.glsl");
	ShaderProgram textRenderSP("shaders/text_render_vs.glsl", "shaders/text_render_fs.glsl");

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
	ResourceManager::setShaderProgram("effectsRenderSP", effectsRenderSP);
	ResourceManager::setShaderProgram("textRenderSP", textRenderSP);

	Texture awesomefaceTex("textures/awesomeface.png");
	Texture backgroundTex("textures/background.jpg");
	Texture solidBlockTex("textures/solid_block.png");
	Texture blockTex("textures/block.png");
	Texture paddleTex("textures/paddle.png");
	Texture particleTex("textures/particle.png");

	Texture powerUpSpeedTex("textures/powerups/powerup_speed.png");
	Texture powerUpStickyTex("textures/powerups/powerup_sticky.png");
	Texture powerUpIncreaseTex("textures/powerups/powerup_increase.png");
	Texture powerUpChaosTex("textures/powerups/powerup_chaos.png");
	Texture powerUpConfuseTex("textures/powerups/powerup_confuse.png");
	Texture powerUpPassThroughTex("textures/powerups/powerup_passthrough.png");

	ResourceManager::setTexture("awesomefaceTex", awesomefaceTex);
	ResourceManager::setTexture("backgroundTex", backgroundTex);
	ResourceManager::setTexture("solidBlock", solidBlockTex);
	ResourceManager::setTexture("destructibleBlock", blockTex);
	ResourceManager::setTexture("paddleTex", paddleTex);
	ResourceManager::setTexture("particleTex", particleTex);

	ResourceManager::setTexture("powerUpSpeedTex", powerUpSpeedTex);
	ResourceManager::setTexture("powerUpStickyTex", powerUpStickyTex);
	ResourceManager::setTexture("powerUpIncreaseTex", powerUpIncreaseTex);
	ResourceManager::setTexture("powerUpChaosTex", powerUpChaosTex);
	ResourceManager::setTexture("powerUpConfuseTex", powerUpConfuseTex);
	ResourceManager::setTexture("powerUpPassThroughTex", powerUpPassThroughTex);

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

	g_Effects = new PostProcessor(effectsRenderSP, m_ScreenWidth, m_ScreenHeight);

	g_TextRenderer = new TextRenderer(m_ScreenWidth, m_ScreenHeight);
	g_TextRenderer->load("fonts/ocraext.ttf", 24);

	g_SoundEngine->play2D("audio/breakout.mp3", true);
}

void Game::processInput(float deltaTime)
{
	if (m_State == GameState::ON_MENU)
	{
		if (m_KeysPressed[GLFW_KEY_ENTER] && !m_KeysProcessed[GLFW_KEY_ENTER])
		{
			m_State = GameState::ACTIVE;

			m_KeysProcessed[GLFW_KEY_ENTER] = true;
		}

		if (m_KeysPressed[GLFW_KEY_W] && !m_KeysProcessed[GLFW_KEY_W])
		{
			m_CurrentLevel = (m_CurrentLevel + 1) % 4;

			m_KeysProcessed[GLFW_KEY_W] = true;
		}

		if (m_KeysPressed[GLFW_KEY_S] && !m_KeysProcessed[GLFW_KEY_S])
		{
			if (m_CurrentLevel > 0)
			{
				m_CurrentLevel = m_CurrentLevel - 1;
			}
			else
			{
				m_CurrentLevel = 3;
			}

			m_KeysProcessed[GLFW_KEY_S] = true;
		}
	}
	else if (m_State == GameState::ACTIVE)
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
	else if (m_State == GameState::VICTORY)
	{
		if (m_KeysPressed[GLFW_KEY_ENTER])
		{
			m_KeysProcessed[GLFW_KEY_ENTER] = true;
			g_Effects->m_ChaosEffect = false;
			m_State = GameState::ON_MENU;
		}
	}
}

void Game::update(float deltaTime)
{
	g_Ball->move(deltaTime, m_ScreenWidth);

	doCollision();  // Do ball collision.

	g_ParticleGenerator->update(deltaTime, *g_Ball, 2, glm::vec2(g_Ball->m_Radius / 2.0f));

	updatePowerUps(deltaTime); // Update all power-ups states.

	if (g_ShakeTime > 0.0f)
	{
		g_ShakeTime -= deltaTime;

		if (g_ShakeTime <= 0.0f)
		{
			g_Effects->m_ShakeEffect = false;
		}
	}

	if (g_Ball->m_Position.y >= m_ScreenHeight)
	{
		m_Lives -= 1;

		if (m_Lives == 0)
		{
			m_State = GameState::ON_MENU;

			resetLevel();
		}

		resetPlayer();
	}

	if (m_State == GameState::ACTIVE && m_Levels[m_CurrentLevel].isCompleted())
	{
		resetLevel();
		resetPlayer();
		
		g_Effects->m_ChaosEffect = true;
		m_State = GameState::VICTORY;
	}
}

void Game::render()
{
	if (m_State == GameState::ON_MENU || m_State == GameState::ACTIVE || m_State == GameState::VICTORY)
	{
		Texture backgroundTex = ResourceManager::getTexture("backgroundTex");

		g_Effects->beginRender();

		{
			g_Renderer->draw(backgroundTex, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(m_ScreenWidth, m_ScreenHeight));

			m_Levels[m_CurrentLevel].draw(*g_Renderer);

			g_Player->draw(*g_Renderer);

			for (PowerUp& powerUp : m_PowerUps)
			{
				if (!powerUp.m_Destroyed)
				{
					powerUp.draw(*g_Renderer);
				}
			}

			g_ParticleGenerator->draw(); // WARNING: Render particles before the ball.

			g_Ball->draw(*g_Renderer);
		}

		g_Effects->endRender();
		g_Effects->render(glfwGetTime());

		std::stringstream ss; ss << m_Lives;
		g_TextRenderer->write("Lives: " + ss.str(), 5.0f, 5.0f, 1.0f);
	}

	if (m_State == GameState::ON_MENU)
	{
		g_TextRenderer->write("Press ENTER to start", 250.0f, m_ScreenHeight / 2, 1.0f);
		g_TextRenderer->write("Press W or S to select level", 245.0f, m_ScreenHeight / 2 + 20.0f, 0.75f);
	}

	if (m_State == GameState::VICTORY)
	{
		g_TextRenderer->write("You WON!!!", 320.0f, m_ScreenHeight / 2 - 20.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		g_TextRenderer->write("Press ENTER to retry or ESC to quit", 130.0f, m_ScreenHeight / 2, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));
	}
}

void Game::setKeyState(int index, bool pressed)
{
	m_KeysPressed[index] = pressed;
}

void Game::setKeyProcessedState(int index, bool processed)
{
	m_KeysProcessed[index] = processed;
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

					spawnPowerUps(brick);

					g_SoundEngine->play2D("audio/bleep.mp3", false);
				}
				else
				{
					g_ShakeTime = 0.05f;

					g_Effects->m_ShakeEffect = true;

					g_SoundEngine->play2D("audio/solid.wav", false);
				}

				Direction direction = std::get<1>(coll);
				glm::vec2 differece = std::get<2>(coll);

				if (!(g_Ball->m_PassThroughEffect && !brick.m_Solid))
				{
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
	}

	// Check collision with the power-ups.
	for (PowerUp& powerUp : m_PowerUps)
	{
		if (!powerUp.m_Destroyed)
		{
			if (powerUp.m_Position.y >= m_ScreenHeight)
			{
				powerUp.m_Destroyed = true;

			}

			if (checkCollision(*g_Player, powerUp))
			{
				activatePowerUp(powerUp);

				powerUp.m_Destroyed = true;
				powerUp.m_Activated = true;

				g_SoundEngine->play2D("audio/powerup.wav", false);
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

		g_Ball->m_Stuck = g_Ball->m_StickyEffect;

		g_SoundEngine->play2D("audio/bleep.wav", false);
	}
}

void Game::resetLevel()
{
	m_Lives = 3;

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

	g_Effects->m_ChaosEffect = false;
	g_Effects->m_ConfuseEffect = false;

	g_Player->m_Color = glm::vec3(1.0f);

	g_Ball->m_Color = glm::vec3(1.0f);
	g_Ball->m_StickyEffect = false;
	g_Ball->m_PassThroughEffect = false;
}

bool Game::checkCollision(GameObject& one, GameObject& two)
{
	bool collisionX = one.m_Position.x + one.m_Size.x >= two.m_Position.x && two.m_Position.x + two.m_Size.x >= one.m_Position.x;
	bool collisionY = one.m_Position.y + one.m_Size.y >= two.m_Position.y && two.m_Position.y + two.m_Size.y >= one.m_Position.y;

	return collisionX && collisionY;
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

void Game::spawnPowerUps(GameObject& block)
{
	if (shouldSpawnPowerUp(75))
	{
		Texture powerUpSpeedTex = ResourceManager::getTexture("powerUpSpeedTex");

		m_PowerUps.push_back(PowerUp("speed", 0.0f, powerUpSpeedTex, block.m_Position, glm::vec3(0.5f, 0.5f, 1.0f)));
	}

	if (shouldSpawnPowerUp(75))
	{
		Texture powerUpStickyTex = ResourceManager::getTexture("powerUpStickyTex");

		m_PowerUps.push_back(PowerUp("sticky", 20.0f, powerUpStickyTex, block.m_Position, glm::vec3(1.0f, 0.5f, 1.0f)));
	}

	if (shouldSpawnPowerUp(75))
	{
		Texture powerUpPassThroughTex = ResourceManager::getTexture("powerUpPassThroughTex");

		m_PowerUps.push_back(PowerUp("passThrough", 10.0f, powerUpPassThroughTex, block.m_Position, glm::vec3(0.5f, 1.0f, 0.5f)));
	}

	if (shouldSpawnPowerUp(75))
	{
		Texture powerUpIncreaseTex = ResourceManager::getTexture("powerUpIncreaseTex");

		m_PowerUps.push_back(PowerUp("padSizeIncrease", 0.0f, powerUpIncreaseTex, block.m_Position, glm::vec3(1.0f, 0.6f, 0.4f)));
	}

	if (shouldSpawnPowerUp(15))
	{
		Texture powerUpChaosTex = ResourceManager::getTexture("powerUpChaosTex");

		m_PowerUps.push_back(PowerUp("chaos", 15.0f, powerUpChaosTex, block.m_Position, glm::vec3(0.9f, 0.25f, 0.25f)));
	}

	if (shouldSpawnPowerUp(15))
	{
		Texture powerUpConfuseTex = ResourceManager::getTexture("powerUpConfuseTex");

		m_PowerUps.push_back(PowerUp("confuse", 15.0f, powerUpConfuseTex, block.m_Position, glm::vec3(1.0f, 0.3f, 0.3f)));
	}
}

void Game::updatePowerUps(float deltaTime)
{
	for (PowerUp& powerUp : m_PowerUps)
	{
		powerUp.m_Position += powerUp.m_Velocity * deltaTime;

		if (powerUp.m_Activated)
		{
			powerUp.m_Duration -= deltaTime;

			if (powerUp.m_Duration <= 0.0f)
			{
				powerUp.m_Activated = false;

				if (powerUp.m_Type == "sticky")
				{
					if (!isOtherPowerUpActive(m_PowerUps, "sticky"))
					{
						g_Player->m_Color = glm::vec3(1.0f);
						g_Ball->m_StickyEffect = false;
					}
				}
				else if (powerUp.m_Type == "passThrough")
				{
					if (!isOtherPowerUpActive(m_PowerUps, "passThrough"))
					{
						g_Ball->m_Color = glm::vec3(1.0f);
						g_Ball->m_PassThroughEffect = false;
					}
				}
				else if (powerUp.m_Type == "chaos")
				{
					if (!isOtherPowerUpActive(m_PowerUps, "chaos"))
					{
						g_Effects->m_ChaosEffect = false;
					}
				}
				else if (powerUp.m_Type == "confuse")
				{
					if (!isOtherPowerUpActive(m_PowerUps, "confuse"))
					{
						g_Effects->m_ConfuseEffect = false;
					}
				}
			}
		}
	}

	// Remove all power-ups that are destroyed and not activated.
	m_PowerUps.erase(std::remove_if(
		m_PowerUps.begin(),
		m_PowerUps.end(),
		[](const PowerUp& powerUp) { return powerUp.m_Destroyed && !powerUp.m_Activated; }
	), m_PowerUps.end());
}

bool Game::shouldSpawnPowerUp(unsigned int chance)
{
	unsigned int random = rand() % chance;

	return random == 0;
}

void Game::activatePowerUp(PowerUp& powerUp)
{
	if (powerUp.m_Type == "speed")
	{
		g_Ball->m_Velocity *= 1.2f;
	}
	else if (powerUp.m_Type == "sticky")
	{
		g_Player->m_Color = glm::vec3(1.0f, 0.5f, 1.0f);
		g_Ball->m_StickyEffect = true;
	}
	else if (powerUp.m_Type == "passThrough")
	{
		g_Ball->m_Color = glm::vec3(1.0f, 0.5f, 0.5f);
		g_Ball->m_PassThroughEffect = true;
	}
	else if (powerUp.m_Type == "padSizeIncrease")
	{
		g_Player->m_Size.x += 50;
	}
	else if (powerUp.m_Type == "chaos")
	{
		if (!g_Effects->m_ConfuseEffect)
		{
			g_Effects->m_ChaosEffect = true; // Only activate if "confuse" wasn't already active.
		}
	}
	else if (powerUp.m_Type == "confuse")
	{
		if (!g_Effects->m_ChaosEffect)
		{
			g_Effects->m_ConfuseEffect = true; // Only activate if "chaos" wasn't already active.
		}
	}
}

bool Game::isOtherPowerUpActive(std::vector<PowerUp>& powerUps, std::string type)
{
	for (const PowerUp& powerUp : powerUps)
	{
		if (powerUp.m_Activated)
		{
			if (powerUp.m_Type == type)
			{
				return true;
			}
		}
	}

	return false;
}
