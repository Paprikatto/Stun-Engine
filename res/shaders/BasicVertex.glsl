#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertColor;

out vec4 v_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	v_color = vec4(vertColor, 1.0);
}