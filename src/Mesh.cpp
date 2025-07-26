#include "Mesh.h"
#include <utility>
#include "Model.h"
#include "Renderer.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures):
    vertices(std::move(vertices)),
    indices(std::move(indices)),
    textures(std::move(textures))
{
    m_VAO = std::make_unique<VertexArray>();
    std::cout << "Creating mesh with " << this->vertices.size() << " vertices, " << this->indices.size() << " indices" << std::endl;
    if (!this->vertices.empty())
    {
        m_VBO = std::make_unique<VertexBuffer>(&this->vertices[0], this->vertices.size() * sizeof(Vertex));
    }
    else
    {
        std::cout << "Mesh has no vertices!" << std::endl;
    }
    if (!this->indices.empty())
    {
        m_IBO = std::make_unique<IndexBuffer>(&this->indices[0], this->indices.size());
    }
    else
    {
        std::cout << "Mesh has no indices!" << std::endl;
    }
    m_VAO->AddBuffer(m_VBO, Model::bufferLayout.value());
    m_VAO->Unbind();
}

void Mesh::Draw() const
{
    m_VAO->Bind();
    m_VBO->Bind();
    m_IBO->Bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
}
