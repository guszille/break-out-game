#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(ShaderProgram& shader)
	: m_VAO(), m_VBO(), m_Shader(shader)
{
    initializeRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
    // TODO: Clear vertex array and buffers from GPU memory.
}

void SpriteRenderer::draw(Texture& texture, glm::vec2 position, float rotation, glm::vec2 scale, glm::vec3 color)
{
    m_Shader.bind();

    glm::mat4 modelMatrix = glm::mat4(1.0f);

    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0.0f));

    modelMatrix = glm::translate(modelMatrix, glm::vec3( 0.5f * scale.x,  0.5f * scale.y, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));

    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 1.0f));

    m_Shader.setUniformMatrix4fv("uModelMatrix", modelMatrix);
    m_Shader.setUniform3f("uSpriteColor", color);
    
    texture.bind(0);

    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void SpriteRenderer::initializeRenderData()
{
    // Position and texture coordinates.
    float quadVertices[] = {
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
