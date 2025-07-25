#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
void main()
{
    //convert the vertex position to world space
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = normalize(normalMatrix * aNormal);
	gl_Position = projection * view * model * vec4(position, 1.0);
}
