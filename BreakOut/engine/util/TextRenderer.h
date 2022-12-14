#pragma once

#pragma once

#include <map>
#include <string>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#if !defined _FREETYPE_INCLUDED
#define _FREETYPE_INCLUDED

#include <ft2build.h>
#include FT_FREETYPE_H
#endif // _FREETYPE_INCLUDED

#include "ResourceManager.h"

#include "../core/ShaderProgram.h"

struct Character
{
	unsigned int m_TextureID;

	glm::ivec2 m_Size, m_Bearing;
	unsigned int m_Advance;
};

class TextRenderer
{
public:
	TextRenderer(unsigned int screenWidth, unsigned int screenHeight);

	~TextRenderer();

	void load(const char* filepath, unsigned int fontSize);
	void write(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));

private:
	std::map<GLchar, Character> m_Characters;
	ShaderProgram m_TextShader;
	unsigned int m_VAO, m_VBO;
};

