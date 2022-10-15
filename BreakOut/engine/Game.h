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
#include "util/ParticleGenerator.h"

#include "GameLevel.h"
#include "GameObject.h"
#include "BallObject.h"

enum GameState
{
	ACTIVE, ON_MENU, VICTORY
};

enum Direction
{
	UP, RIGHT, DOWN, LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

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
	void doCollision();

	void resetLevel();
	void resetPlayer();

private:
	unsigned int m_ScreenWidth, m_ScreenHeight;
	bool m_KeysPressed[1024];
	GameState m_State;

	std::vector<GameLevel> m_Levels;
	unsigned int m_CurrentLevel;

	Collision checkCollision(BallObject& ball, GameObject& aabb);
	Direction getCollisionDirection(glm::vec2 target);
};
