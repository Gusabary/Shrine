#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 cameraPos;

void main()
{    
	float fogDistance = sqrt(pow(cameraPos.x - FragPos.x, 2) + pow(cameraPos.z - FragPos.z, 2));
	float fogMax = 40.0f;
	float fogMin = 5.0f;
	float fogFactor = (fogMax - fogDistance) / (fogMax - fogMin);
	fogFactor = clamp(fogFactor, 0.3f, 1.0f);
	vec4 fogColor = vec4(0.8f, 0.8f, 0.8f, 1.0f);
    FragColor = mix(fogColor, texture(texture_diffuse1, TexCoords), fogFactor);

    //FragColor = texture(texture_diffuse1, TexCoords);
}