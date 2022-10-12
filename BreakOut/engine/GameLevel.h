#pragma once

#include <vector>
#include <fstream>

#include "util/ResourceManager.h"
#include "util/SpriteRenderer.h"

#include "GameObject.h"

class GameLevel
{
public:
	GameLevel();

	~GameLevel();

	void load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight);

	void draw(SpriteRenderer& renderer);

	bool isCompleted();

private:
	std::vector<GameObject> m_Bricks;

	void initialize(std::vector<std::vector<unsigned int>> tileSetData, unsigned int levelWidth, unsigned int levelHeight);
};
