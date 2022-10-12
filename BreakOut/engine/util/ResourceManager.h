#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "../core/ShaderProgram.h"
#include "../core/Texture.h"

class ResourceManager
{
public:
	static void setShaderProgram(std::string key, ShaderProgram shader);
	static ShaderProgram getShaderProgram(std::string key);

	static void setTexture(std::string key, Texture texture);
	static Texture getTexture(std::string key);

	static void clear();

private:
	ResourceManager();
	~ResourceManager();

	static std::map<std::string, ShaderProgram> m_Shaders;
	static std::map<std::string, Texture> m_Textures;
};
