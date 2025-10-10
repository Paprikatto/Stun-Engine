#include "Model.h"

#include <bemapiset.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/postprocess.h>
#include <ext/matrix_transform.hpp>

#include "Camera.h"
#include "Camera.h"
#include "Camera.h"
#include "Utilities.h"

std::optional<VertexBufferLayout> Model::bufferLayout;

Model::Model(const std::string& path, Shader &shader): m_Shader(shader)
{
    m_Directory = path.substr(0, path.find_last_of('/') + 1);
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
    for (const auto & mesh : m_Meshes)
    {
        mesh.Draw(m_Shader, m_ModelMatrix);
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
    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}

void Model::processNode(aiNode* node, const aiScene* scene, const glm::mat4& parentTransform)
{
    auto nodeTransform = AiMatrix4x4ToGlm(node->mTransformation);
    auto globalTransform = parentTransform * nodeTransform;
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.emplace_back(processMesh(mesh, scene, globalTransform));
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, globalTransform);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, const glm::mat4 nodeTransform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*> textures;

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
    //Process materials and textures
    Material material{};
    if (mesh->mMaterialIndex > 0)
    {
        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];

        aiColor3D color(0.0f, 0.0f, 0.0f);
        //get material diffuse color
        if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
        {
            material.diffuseColor = glm::vec3(color.r, color.g, color.b);
        }
        //get material specular color
        if (AI_SUCCESS == ai_material->Get(AI_MATKEY_COLOR_SPECULAR, color))
        {
            material.specularColor = glm::vec3(color.r, color.g, color.b);
        }
        
        //get material shininess
        float shininess = 0.0f;
        if (AI_SUCCESS == ai_material->Get(AI_MATKEY_SHININESS, shininess))
        {
            material.shininess = shininess;
        }
        
        // Process diffuse maps
        material.diffuseMaps = loadMaterialTextures(ai_material, aiTextureType_DIFFUSE);
        // Process specular maps
        material.specularMaps = loadMaterialTextures(ai_material, aiTextureType_SPECULAR);
    }

    return {vertices, indices, material, nodeTransform};
}

std::vector<const Texture*> Model::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type)
{
    std::vector<const Texture*> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //check if texture is already loaded
        auto texPath = m_Directory + std::string(str.C_Str());
        auto texture = findTexture(texPath);
        textures.push_back(texture);
    }
    return textures;
}
/// @brief check if texture was already loaded, if not load it
const Texture* Model::findTexture(const std::string& path)
{
    for (const auto& texture : m_TexturesLoaded)
    {
        if (texture->GetFilePath() == path)
        {
            return texture.get();
        }
    }
    m_TexturesLoaded.emplace_back(std::make_unique<Texture>(path));
    std::cout << "Loaded texture: " << path << std::endl;
    return m_TexturesLoaded.back().get();
}
