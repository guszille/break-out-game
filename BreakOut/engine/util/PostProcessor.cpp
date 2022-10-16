#include "PostProcessor.h"

PostProcessor::PostProcessor(ShaderProgram& shader, unsigned int width, unsigned int height)
	: m_PostProcessingShader(shader), m_Texture(), m_Width(width), m_Height(height), m_ChaosEffect(false), m_ConfuseEffect(false), m_ShakeEffect(false)
{
	float offset = 1.0f / 300.0f;

	float offsets[9][2] = {
		{ -offset,  offset  },  // top-left
		{  0.0f,    offset  },  // top-center
		{  offset,  offset  },  // top-right
		{ -offset,  0.0f    },  // center-left
		{  0.0f,    0.0f    },  // center-center
		{  offset,  0.0f    },  // center-right
		{ -offset, -offset  },  // bottom-left
		{  0.0f,   -offset  },  // bottom-center
		{  offset, -offset  }   // bottom-right
	};

	int edgeKernel[9] = {
		-1, -1, -1,
		-1,  8, -1,
		-1, -1, -1
	};

	float blurKernel[9] = {
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
		2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
		1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
	};

	glGenFramebuffers(1, &m_MSFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);

	glGenRenderbuffers(1, &m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_RBO);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR] POST PROCESSOR: Failed to initialize MSFBO." << std::endl;
	}

	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	m_Texture = Texture(width, height);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Texture.getId(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "[ERROR] POST PROCESSOR: Failed to initialize FBO." << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initializeRenderData();

	m_PostProcessingShader.bind();

	m_PostProcessingShader.setUniform1i("uScene", 0);
	m_PostProcessingShader.setUniform2fv("uOffsets", 9, (float*)offsets);
	m_PostProcessingShader.setUniform1iv("uEdgeKernel", 9, edgeKernel);
	m_PostProcessingShader.setUniform1fv("uBlurKernel", 9, blurKernel);

	m_PostProcessingShader.unbind();
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::beginRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_MSFBO);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::endRender()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MSFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);

	glBlitFramebuffer(0, 0, m_Width, m_Height, 0, 0, m_Width, m_Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessor::render(float time)
{
	m_PostProcessingShader.bind();

	m_PostProcessingShader.setUniform1f("uTime", time);
	m_PostProcessingShader.setUniform1i("uChaos", m_ChaosEffect);
	m_PostProcessingShader.setUniform1i("uConfuse", m_ConfuseEffect);
	m_PostProcessingShader.setUniform1i("uShake", m_ShakeEffect);

	m_Texture.bind(0);

	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	m_PostProcessingShader.unbind();
}

void PostProcessor::initializeRenderData()
{
	// Position and texture coordinates.
	float quadVertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f, 0.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 1.0f
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
