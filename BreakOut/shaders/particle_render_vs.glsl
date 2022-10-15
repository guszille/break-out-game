#version 330 core

layout (location = 0) in vec4 aVertex; // <vec2 aPosition, vec2 aTexCoords>

uniform mat4 uProjectionMatrix;
uniform vec2 uOffset;
uniform vec4 uColor;

out vec2 ioTexCoords;
out vec4 ioParticleColor;

void main()
{
    float scale = 10.0;

    gl_Position = uProjectionMatrix * vec4((aVertex.xy * scale) + uOffset, 0.0, 1.0);

    ioTexCoords = aVertex.zw;
    ioParticleColor = uColor;
}
