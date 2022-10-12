#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/ShaderProgram.h"
#include "../core/Texture.h"

class SpriteRenderer
{
public:
	SpriteRenderer(ShaderProgram& shader);

	~SpriteRenderer();

	void draw(Texture& texture, glm::vec2 position, float rotation = 0.0f, glm::vec2 scale = glm::vec2(10.0f, 10.0f), glm::vec3 color = glm::vec3(1.0f));

private:
	unsigned int m_VAO, m_VBO;
	ShaderProgram m_Shader;

	void initializeRenderData();
};
