#pragma once
#include "glm.hpp"
#include <vector>
#include <string>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};
struct Texture{
    unsigned int id;
    std::string type;   
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, VertexBufferLayout layout);
private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;
};