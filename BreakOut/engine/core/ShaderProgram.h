#pragma once

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const char* vertexShaderFilepath, const char* fragmentShaderFilepath);
	ShaderProgram(const char* vertexShaderFilepath, const char* geometryShaderFilepath, const char* fragmentShaderFilepath);
	
	~ShaderProgram();

	void clear();

	void bind();
	void unbind();

	void setUniform1i(const char* uniformName, const int& data);

	void setUniform1f(const char* uniformName, const float& data);
	void setUniform2f(const char* uniformName, const glm::vec2& data);
	void setUniform3f(const char* uniformName, const glm::vec3& data);
	void setUniform4f(const char* uniformName, const glm::vec4& data);

	void setUniform1iv(const char* uniformName, int count, const int* data);

	void setUniform1fv(const char* uniformName, int count, const float* data);
	void setUniform2fv(const char* uniformName, int count, const float* data);

	void setUniformMatrix4fv(const char* uniformName, const glm::mat4& data);

private:
	unsigned int m_ID;

	const std::string readShaderSource(const char* filepath);
	const unsigned int createShader(const char* filepath, int shaderType);
};
