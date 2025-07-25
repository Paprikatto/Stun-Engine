#include "Model.h"

#include <bemapiset.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/postprocess.h>
#include <ext/matrix_transform.hpp>

std::optional<VertexBufferLayout> Model::bufferLayout;

Model::Model(const std::string& path, Shader &shader): m_Shader(shader)
{
    if (!bufferLayout.has_value())
    {
        bufferLayout.emplace(VertexBufferLayout());
        bufferLayout->Push<float>(3); // position
        bufferLayout->Push<float>(3); // normal
        bufferLayout->Push<float>(2); // texture coordinates
    }
    loadModel(path);
}

void Model::Draw() const
{
    m_Shader.Bind();
    m_Shader.SetUniformMat4f("model", m_ModelMatrix);
    const glm::mat3 normal_matrix = glm::transpose(glm::inverse(m_ModelMatrix));
    m_Shader.SetUniformMat3f("normalMatrix", normal_matrix);
    for (const auto & mesh : m_Meshes)
    {
        mesh.Draw();
    }
}

void Model::SetPosition(const glm::vec3& position)
{
    m_ModelMatrix = glm::translate(glm::mat4(1.0f), position);
}

void Model::RotateDegrees(const float degrees, const glm::vec3& axis)
{
    m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(degrees), axis);
}

void Model::RotateRadians(const float radians, const glm::vec3& axis)
{
    m_ModelMatrix = glm::rotate(m_ModelMatrix, radians, axis);
}

void Model::SetScale(const glm::vec3& scale)
{
    m_ModelMatrix = glm::scale(m_ModelMatrix, scale);
}

void Model::SetScale(const float scale)
{
    const glm::vec3 scale_vec(scale, scale, scale);
    m_ModelMatrix = glm::scale(m_ModelMatrix, scale_vec);
}

void Model::loadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        throw std::runtime_error("ERROR::ASSIMP:: " + std::string(importer.GetErrorString()));
    }
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.emplace_back(processMesh(mesh));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // Process vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex{};
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else
        {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }
    // Process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    //TODO: Process materials and textures

    return {vertices, indices, textures};
}
