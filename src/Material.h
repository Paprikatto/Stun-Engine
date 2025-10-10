#pragma once
#include "glm.hpp"
#include "Texture.h"
#include <vector>

struct Material {
    glm::vec3 diffuseColor{1.0f, 1.0f, 1.0f};
    glm::vec3 specularColor{1.0f, 1.0f, 1.0f};
    float shininess = 32.0f;
    float opacity = 1.0f; 

    std::vector<const Texture*> diffuseMaps;
    std::vector<const Texture*> specularMaps;
};