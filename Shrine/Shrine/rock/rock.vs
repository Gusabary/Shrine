#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec2 texCoords;
} vs_out;

out vec3 iNormal;
out vec3 iFragPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vs_out.texCoords = aTexCoords;
	iNormal = mat3(transpose(inverse(model))) * aNormal;  
	iFragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0); 
}