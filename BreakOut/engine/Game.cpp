#include "Game.h"

SpriteRenderer* g_Renderer;

GameObject* g_Player;

const glm::vec2 g_PlayerSize(100.0f, 20.0f);
const float g_PlayerVelocity(500.0f);

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

	spriteRenderSP.bind();
	spriteRenderSP.setUniformMatrix4fv("uProjectionMatrix", projectionMatrix);
	spriteRenderSP.setUniform1i("uTexture", 0);
	spriteRenderSP.unbind();

	ResourceManager::setShaderProgram("spriteRenderSP", spriteRenderSP);

	Texture backgroundTex("textures/background.jpg");
	Texture solidBlockTex("textures/solid_block.png");
	Texture blockTex("textures/block.png");
	Texture paddleTex("textures/paddle.png");

	ResourceManager::setTexture("backgroundTex", backgroundTex);
	ResourceManager::setTexture("solidBlock", solidBlockTex);
	ResourceManager::setTexture("destructibleBlock", blockTex);
	ResourceManager::setTexture("paddleTex", paddleTex);

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

	g_Player = new GameObject(ResourceManager::getTexture("paddleTex"), playerPosition, g_PlayerSize);

	g_Renderer = new SpriteRenderer(spriteRenderSP);
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
			}
		}

		if (m_KeysPressed[GLFW_KEY_D])
		{
			if (g_Player->m_Position.x <= m_ScreenWidth - g_Player->m_Size.x)
			{
				g_Player->m_Position.x += velocity;
			}
		}
	}
}

void Game::update(float deltaTime)
{
}

void Game::render()
{
	if (m_State == GameState::ACTIVE)
	{
		Texture backgroundTex = ResourceManager::getTexture("backgroundTex");

		g_Renderer->draw(backgroundTex, glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(m_ScreenWidth, m_ScreenHeight));

		m_Levels[m_CurrentLevel].draw(*g_Renderer);

		g_Player->draw(*g_Renderer);
	}
}

void Game::setKeyState(int index, bool pressed)
{
	m_KeysPressed[index] = pressed;
}
