#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/ShaderProgram.h"
#include "core/Texture.h"

#include "util/ResourceManager.h"
#include "util/SpriteRenderer.h"

#include "GameLevel.h"

enum GameState
{
	ACTIVE, ON_MENU, VICTORY
};

class Game
{
public:
	Game(unsigned int width, unsigned int height);
	~Game();

	void setup();
	void processInput(float deltaTime);
	void update(float deltaTime);
	void render();

	void setKeyState(int index, bool pressed);

private:
	unsigned int m_ScreenWidth, m_ScreenHeight;
	bool m_KeysPressed[1024];
	GameState m_State;

	std::vector<GameLevel> m_Levels;
	unsigned int m_CurrentLevel;
};
