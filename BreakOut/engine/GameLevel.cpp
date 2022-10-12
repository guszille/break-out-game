#include "GameLevel.h"

GameLevel::GameLevel()
{
}

GameLevel::~GameLevel()
{
}

void GameLevel::load(const char* filepath, unsigned int levelWidth, unsigned int levelHeight)
{
	m_Bricks.clear();

	std::ifstream fileStream(filepath);
	std::vector<std::vector<unsigned int>> tileSetData;

	unsigned int code;
	std::string line;

	if (fileStream)
	{
		while (std::getline(fileStream, line))
		{
			std::istringstream stringStream(line);
			std::vector<unsigned int> row;

			while (stringStream >> code)
			{
				row.push_back(code);
			}

			tileSetData.push_back(row);
		}

		if (tileSetData.size() > 0)
		{
			initialize(tileSetData, levelWidth, levelHeight);
		}
	}
}

void GameLevel::draw(SpriteRenderer& renderer)
{
	for (GameObject& tile : m_Bricks)
	{
		if (!tile.m_Destroyed)
		{
			tile.draw(renderer);
		}
	}
}

bool GameLevel::isCompleted()
{
	for (GameObject& tile : m_Bricks)
	{
		if (!tile.m_Solid && !tile.m_Destroyed)
		{
			return false;
		}
	}

	return true;
}

void GameLevel::initialize(std::vector<std::vector<unsigned int>> tileSetData, unsigned int levelWidth, unsigned int levelHeight)
{
	unsigned int rows = tileSetData.size();
	unsigned int columns = tileSetData[0].size();

	float unitWidth = levelWidth / (float)columns;
	float unitHeight = levelHeight / (float)rows;

	for (unsigned int i = 0; i < rows; ++i)
	{
		for (unsigned int j = 0; j < columns; ++j)
		{
			if (tileSetData[i][j] == 1)
			{
				glm::vec2 position(unitWidth * j, unitHeight * i);
				glm::vec2 size(unitWidth, unitHeight);
				glm::vec3 color(0.8f, 0.8f, 0.8f);

				GameObject gObject(ResourceManager::getTexture("solidBlock"), position, size, color);
				
				gObject.m_Solid = true;
				
				m_Bricks.push_back(gObject);
			}
			else if (tileSetData[i][j] > 1)
			{
				glm::vec2 position(unitWidth * j, unitHeight * i);
				glm::vec2 size(unitWidth, unitHeight);
				glm::vec3 color(1.0f);

				switch (tileSetData[i][j])
				{
				case 2:
					color = glm::vec3(0.2f, 0.6f, 1.0f);
					break;

				case 3:
					color = glm::vec3(0.0f, 0.7f, 0.0f);
					break;

				case 4:
					color = glm::vec3(0.8f, 0.8f, 0.4f);
					break;

				case 5:
					color = glm::vec3(1.0f, 0.5f, 0.0f);
					break;

				default:
					break;
				}

				GameObject gObject(ResourceManager::getTexture("destructibleBlock"), position, size, color);

				m_Bricks.push_back(gObject);
			}
		}
	}
}
