#version 330 core

layout (location = 0) out vec4 color;

in vec3 Normal;
in vec3 FragPos;

uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main(){
    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
	color = vec4(result, 1.0);
}
