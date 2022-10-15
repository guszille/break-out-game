#pragma once

#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../core/ShaderProgram.h"
#include "../core/Texture.h"

#include "../GameObject.h"

struct Particle
{
	glm::vec2 m_Position, m_Velocity;
	glm::vec4 m_Color;
	float m_Life;

	Particle() : m_Position(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f) {}
};

class ParticleGenerator
{
public:
	ParticleGenerator(ShaderProgram& shader, Texture& texture, unsigned int amount);

	~ParticleGenerator();

	void update(float deltaTime, GameObject& reference, unsigned int numberOfNewParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));

	void draw();

private:
	std::vector<Particle> m_Particles;
	unsigned int m_ParticlesAmount;

	ShaderProgram m_Shader;
	Texture m_Texture;
	unsigned int m_VAO, m_VBO;

	void initializeRenderData();
	unsigned int getFirstUnusedParticle();
	void respawnParticle(Particle& particle, GameObject& reference, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
