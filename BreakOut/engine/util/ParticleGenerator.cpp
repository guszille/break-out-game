#include "ParticleGenerator.h"

unsigned int g_LastUsedParticle = 0;

ParticleGenerator::ParticleGenerator(ShaderProgram& shader, Texture& texture, unsigned int amount)
	: m_Shader(shader), m_Texture(texture), m_ParticlesAmount(amount)
{
	initializeRenderData();
}

ParticleGenerator::~ParticleGenerator()
{
}

void ParticleGenerator::update(float deltaTime, GameObject& reference, unsigned int numberOfNewParticles, glm::vec2 offset)
{
	for (unsigned int i = 0; i < numberOfNewParticles; ++i)
	{
		int unusedParticle = getFirstUnusedParticle();

		respawnParticle(m_Particles[unusedParticle], reference, offset);
	}

	for (unsigned int i = 0; i < m_ParticlesAmount; ++i)
	{
		Particle& particle = m_Particles[i];

		particle.m_Life -= deltaTime;

		if (particle.m_Life > 0.0f)
		{
			particle.m_Position -= particle.m_Velocity * deltaTime;
			particle.m_Color.a -= deltaTime * 2.5f;
		}
	}
}

void ParticleGenerator::draw()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_Shader.bind();

	for (Particle& particle : m_Particles)
	{
		if (particle.m_Life > 0.0f)
		{
			m_Shader.setUniform2f("uOffset", particle.m_Position);
			m_Shader.setUniform4f("uColor", particle.m_Color);
			
			m_Texture.bind(0);

			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);
		}
	}

	m_Shader.unbind();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::initializeRenderData()
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

	// Initialize the vector of particles too.
	for (unsigned int i = 0; i < m_ParticlesAmount; ++i)
	{
		m_Particles.push_back(Particle());
	}
}

unsigned int ParticleGenerator::getFirstUnusedParticle()
{
	for (unsigned int i = g_LastUsedParticle; i < m_ParticlesAmount; ++i)
	{
		if (m_Particles[i].m_Life <= 0.0f)
		{
			g_LastUsedParticle = i;

			return i;
		}
	}

	for (unsigned int i = 0; i < g_LastUsedParticle; ++i)
	{
		if (m_Particles[i].m_Life <= 0.0f)
		{
			g_LastUsedParticle = i;

			return i;
		}
	}

	g_LastUsedParticle = 0;

	return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, GameObject& reference, glm::vec2 offset)
{
	float randomPosition = ((rand() % 100) - 50) / 10.0f;
	float randomColor = 0.5f + ((rand() % 100) / 100.0f);

	particle.m_Position = reference.m_Position + randomPosition + offset;
	particle.m_Color = glm::vec4(randomColor, randomColor, randomColor, 1.0f);
	particle.m_Life = 1.0f;
	particle.m_Velocity = reference.m_Velocity * 0.1f;
}
