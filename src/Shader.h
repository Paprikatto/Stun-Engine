#pragma once
#include "glm/glm.hpp"
#include <string>
#include <unordered_map>

class Shader
{
private:
    enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
    unsigned int m_RendererID;
    std::string m_Filepath;
    std::unordered_map<std::string, int> m_UniformLocationCache;
public:
    Shader(const std::string& filepath);
    Shader(const std::string& vertexShader, const std::string& fragmentShader);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetVec3f(const std::string& name, float v0, float v1, float v2);
    void SetVec4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
    int GetUniformLocation(const std::string& name);
    void ParseShader(std::string& vertexShader, std::string& fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader);
    void LoadFile(const std::string& filepath, std::string& target);
};
