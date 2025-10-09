#version 330 core

layout (location = 0) out vec4 color;

struct Material {
    vec3 diffuseColor; //kolor bazowy
    vec3 specularColor; //kolor odbicia
    float shininess;
};
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform Material material;
uniform float ambientStrength;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform bool useDiffuseMap;
uniform bool useSpecularMap;

void main(){
    // ambient
    vec3 ambient = ambientStrength * lightColor;

    // vectors 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.8;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 finalDiffuseColor = useDiffuseMap ? texture(diffuseMap, TexCoords).rgb : material.diffuseColor;
    vec3 finalSpecularColor = useSpecularMap ? texture(specularMap, TexCoords).rgb : material.specularColor;

    vec3 result = ambient * finalDiffuseColor + diffuse * finalDiffuseColor + specular * finalSpecularColor;
	color = vec4(result, 1.0);
}
