#version 330 core

in vec2 ioTexCoords;

uniform sampler2D uTexture;
uniform vec3 uSpriteColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(uSpriteColor, 1.0) * texture(uTexture, ioTexCoords);
}
