#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>
#include <optional>
#include <memory>

class Model {
public:
    explicit Model(const std::string& path, Shader &shader);
    void Draw() const;
    static std::optional<VertexBufferLayout> bufferLayout;
    void SetPosition(const glm::vec3& position);
    void RotateDegrees(float degrees, const glm::vec3& axis);
    void RotateRadians(float radians, const glm::vec3& axis);
    void SetScale(const glm::vec3& scale);
    void SetScale(float scale);
private:
    std::vector<Mesh> m_Meshes;
    std::vector<std::unique_ptr<Texture>> m_TexturesLoaded; // to avoid loading duplicate textures
    Shader &m_Shader;
    glm::mat4 m_ModelMatrix = glm::mat4(1.0f); // Default model matrix
    std::string m_Directory;
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    [[nodiscard]] std::vector<const Texture*> loadMaterialTextures(const aiMaterial* mat, aiTextureType type);
    const Texture* findTexture(const std::string& path);
};