#version 330 core

in vec2 ioTexCoords;
in vec4 ioParticleColor;

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, ioTexCoords) * ioParticleColor;
}
