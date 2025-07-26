#pragma once
#include <string>
#include <vector>
#include "Mesh.h"
#include <assimp/scene.h>

class Model {
public:
    explicit Model(const std::string& path, Shader &shader);
    void Draw() const;
    static std::optional<VertexBufferLayout> bufferLayout;
private:
    std::vector<Mesh> m_Meshes;
    Shader &m_Shader;
    glm::mat4 m_ModelMatrix = glm::mat4(1.0f); // Default model matrix
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    static Mesh processMesh(aiMesh* mesh);
};