#version 330 core

layout (location = 0) out vec4 color;
in vec4 v_color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
	color = v_color;
}