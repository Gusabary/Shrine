#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
	vec4 fogColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    FragColor = mix(fogColor, texture(skybox, TexCoords), 0.2f);
}