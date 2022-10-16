#version 330 core

in vec2 ioTexCoords;

uniform sampler2D uScene;

uniform bool uChaos;
uniform bool uConfuse;
uniform bool uShake;

uniform vec2 uOffsets[9];
uniform int uEdgeKernel[9];
uniform float uBlurKernel[9];

out vec4 FragColor;

void main()
{
	vec3 sample[9];

	FragColor = vec4(0.0);

	if (uChaos || uShake)
	{
		for(int i = 0; i < 9; i++)
		{
            sample[i] = vec3(texture(uScene, ioTexCoords.st + uOffsets[i]));
		}
	}

	if (uChaos)
	{
		for(int i = 0; i < 9; i++)
		{
            FragColor += vec4(sample[i] * uEdgeKernel[i], 0.0f);
		}

        FragColor.a = 1.0f;
	}
	else if (uConfuse)
	{
		FragColor = vec4(1.0 - texture(uScene, ioTexCoords).rgb, 1.0);
	}
	else if (uShake)
    {
        for(int i = 0; i < 9; i++)
		{
            FragColor += vec4(sample[i] * uBlurKernel[i], 0.0f);
		}

        FragColor.a = 1.0f;
    }
    else
    {
        FragColor =  texture(uScene, ioTexCoords);
    }
}
