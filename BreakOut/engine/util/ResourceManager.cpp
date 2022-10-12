#include "ResourceManager.h"

std::map<std::string, ShaderProgram> ResourceManager::m_Shaders;
std::map<std::string, Texture> ResourceManager::m_Textures;

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	// TODO: Clear all shader programs and textures from GPU memory.
}

void ResourceManager::setShaderProgram(std::string key, ShaderProgram shader)
{
	m_Shaders[key] = shader;
}

ShaderProgram ResourceManager::getShaderProgram(std::string key)
{
	return m_Shaders[key];
}

void ResourceManager::setTexture(std::string key, Texture texture)
{
	m_Textures[key] = texture;
}

Texture ResourceManager::getTexture(std::string key)
{
	return m_Textures[key];
}

void ResourceManager::clear()
{
	for (std::pair<std::string, ShaderProgram> tuple : m_Shaders)
	{
		tuple.second.clear();
	}

	for (std::pair<std::string, Texture> tuple : m_Textures)
	{
		tuple.second.clear();
	}
}
