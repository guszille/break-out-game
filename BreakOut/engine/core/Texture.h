#pragma once

#include <iostream>

#include <glad/glad.h>

#if !defined _STB_IMAGE_INCLUDED
#define _STB_IMAGE_INCLUDED

#include <stbi/stb_image.h>
#endif // _STB_IMAGE_INCLUDED

class Texture
{
public:
	Texture();
	Texture(const char* filepath, const bool gammaCorrection = false);
	Texture(unsigned int width, unsigned int height);
	
	~Texture();

	void clear();

	void bind(int unit);
	void unbind();

	unsigned int getId();

private:
	unsigned int m_ID;
	int m_Width, m_Height, m_ColorChannels;
};
