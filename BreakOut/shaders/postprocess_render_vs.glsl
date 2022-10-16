#version 330 core

layout (location = 0) in vec4 aVertex; // <vec2 aPosition, vec2 aTexCoords>

uniform float uTime;
uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;

out vec2 ioTexCoords;

void main()
{
    gl_Position = vec4(aVertex.xy, 0.0f, 1.0f);

    ioTexCoords = aVertex.zw;

    if (uChaos)
    {
        float strength = 0.30;

        ioTexCoords = vec2(ioTexCoords.x + sin(uTime) * strength, ioTexCoords.y + cos(uTime) * strength);
    }
    else if (uConfuse)
    {
        ioTexCoords = vec2(1.0 - ioTexCoords.x, 1.0 - ioTexCoords.y);
    }

    if (uShake)
    {
        float strength = 0.01;

        gl_Position.x += cos(uTime * 10) * strength;
        gl_Position.y += cos(uTime * 15) * strength;
    }
}
