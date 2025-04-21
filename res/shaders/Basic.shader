#shader vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 vertColor;

out vec2 v_TexCoord;
out vec4 v_color;

uniform vec4 m_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	v_color = vec4(vertColor, 1.0);
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main(){
//	vec4 texColor = texture(u_Texture, v_TexCoord);
//	color = vec4(texColor);
	color = v_color;
}