#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float time;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;

	float time = clamp(time, 0.0f, 5.0f);
	float fogFactor = time < 1.0f ? 0.0f : (time - 1.0f) / 4;

	vec4 fogColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    FragColor = mix(fogColor, vec4(col, 1.0), fogFactor);
} 