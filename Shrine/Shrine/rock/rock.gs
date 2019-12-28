#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec2 texCoords;
} gs_in[];

in vec3 iNormal[];
in vec3 iFragPos[];

out vec2 TexCoords; 
out vec3 Normal;
out vec3 FragPos;

uniform bool doExplode;
uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 6.0;
    vec3 direction = normal * time * magnitude; 
	if (doExplode)
		return position + vec4(direction, 0.0);
	return position;
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
	Normal = (iNormal[0] + iNormal[1] + iNormal[2]) / 3;
	FragPos = (iFragPos[0] + iFragPos[1] + iFragPos[2]) / 3;

    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].texCoords;
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = gs_in[2].texCoords;
    EmitVertex();
    EndPrimitive();
}