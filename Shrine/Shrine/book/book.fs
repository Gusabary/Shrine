#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct Light {
    vec3 position;
	vec3 color;

	vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
uniform sampler2D normalMap; 
uniform float cameraPos;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb * light.color;
    
    // diffuse 
    vec3 norm = normalize(texture(normalMap, TexCoords).rgb * 2.0 - 1.0);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb * light.color;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb * light.color;  

	// spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;
    
    // attenuation
    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    //FragColor = vec4(result, 1.0);
	float fogDistance = cameraPos - FragPos.z;
	float fogMax = 10.0f;
	float fogMin = 2.0f;
	float fogFactor = (fogMax - fogDistance) / (fogMax - fogMin);
	fogFactor = clamp(fogFactor, 0.0f, 1.0f);
	vec4 fogColor = vec4(0.8f, 0.8f, 0.8f, 0.0f);
	FragColor = mix(fogColor, vec4(result, 1.0), fogFactor);
}