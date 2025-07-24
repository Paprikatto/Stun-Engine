#include "Mesh.h"
#include <GL/glew.h>
#include <utility>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, const VertexBufferLayout &layout):
    vertices(std::move(vertices)),
    indices(std::move(indices)),
    textures(std::move(textures)),
    m_VAO(VertexArray())
{
    m_VBO = VertexBuffer(&this->vertices[0], this->vertices.size() * sizeof(Vertex));
    m_IBO = IndexBuffer(&this->indices[0], this->indices.size() * sizeof(unsigned int));
    m_VAO.AddBuffer(m_VBO, layout);
    m_VAO.Unbind();
}