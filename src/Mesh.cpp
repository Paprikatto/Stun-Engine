#include "Mesh.h"
#include <utility>
#include "Model.h"
#include "Renderer.h"
#include <iostream>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, Material material):
    vertices(std::move(vertices)),
    indices(std::move(indices)),
    m_Material(std::move(material))
{
    m_VAO = std::make_unique<VertexArray>();
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

void Mesh::Draw(Shader& shader) const
{
    //use texture or material color
    if (!m_Material.diffuseMaps.empty()) {
        shader.SetUniform1i("useDiffuseMap", 1); 
        shader.SetUniform1i("diffuseMap", 0);
        m_Material.diffuseMaps[0]->Bind();
    } else {
        shader.SetUniform1i("useDiffuseMap", 0); // false
        shader.SetVec3f("material.diffuseColor", m_Material.diffuseColor.r, m_Material.diffuseColor.g, m_Material.diffuseColor.b);
    }
    
    if (!m_Material.specularMaps.empty()) {
        shader.SetUniform1i("useSpecularMap", 1);
        shader.SetUniform1i("specularMap", 1); 
        m_Material.specularMaps[0]->Bind(1);
    } else {
        shader.SetUniform1i("useSpecularMap", 0); 
        shader.SetVec3f("material.specularColor", m_Material.specularColor.r, m_Material.specularColor.g, m_Material.specularColor.b);
    }

    shader.SetUniform1f("material.shininess", m_Material.shininess);

    m_VAO->Bind();
    m_VBO->Bind();
    m_IBO->Bind();
    GL_CALL(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));
    
    //unbind textures
    Texture::Unbind();
}
