#pragma once

#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>

#include "SpriteRenderer.h"

#include "../core/ShaderProgram.h"
#include "../core/Texture.h"

class PostProcessor
{
public:
	unsigned int m_Width, m_Height;
	bool m_ChaosEffect, m_ConfuseEffect, m_ShakeEffect;

	ShaderProgram m_PostProcessingShader;
	Texture m_Texture;

	PostProcessor(ShaderProgram& shader, unsigned int width, unsigned int height);

	~PostProcessor();

	void beginRender();
	void endRender();

	void render(float time);

private:
	unsigned int m_MSFBO, m_FBO; // Multisampled and regular FBO.
	unsigned int m_RBO; // Render buffer object.
	unsigned int m_VAO, m_VBO; // Vertex array object and vertex buffer object.

	void initializeRenderData();
};
