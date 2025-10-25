#pragma once

#include "glm.hpp"
#include <vector>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include <memory>
#include "Material.h"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    glm::mat4 m_transformMatrix = glm::mat4(1.0f);
    Material m_Material;
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material, glm::mat4 transform = glm::mat4(1.0f));

    void Draw(Shader& shader, const glm::mat4 &modelMatrix) const;
private:
    std::unique_ptr<VertexArray> m_VAO;
    std::shared_ptr<VertexBuffer> m_VBO;
    std::unique_ptr<IndexBuffer> m_IBO;
};
