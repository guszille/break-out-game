#pragma once

#include <vector>
#include <fstream>

#include "util/ResourceManager.h"
#include "util/SpriteRenderer.h"

#include "GameObject.h"

class GameLevel
{
public:
	std::vector<GameObject> m_Bricks;

	GameLevel();

	~GameLevel();

	void load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight);

	void draw(SpriteRenderer& renderer);

	bool isCompleted();

private:
	void initialize(std::vector<std::vector<unsigned int>> tileSetData, unsigned int levelWidth, unsigned int levelHeight);
};
