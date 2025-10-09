#pragma once
#include "Renderer.h"
enum TextureType
{
    DIFFUSE,
    SPECULAR
};
class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_TextureData;
    int m_Width, m_Height, m_BPP;
    TextureType m_Type;
public:
    explicit Texture(const std::string& filePath, TextureType type = DIFFUSE);
    Texture (const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;
    Texture(Texture&& other) noexcept : m_RendererID(other.m_RendererID), m_FilePath(std::move(other.m_FilePath)),
    m_TextureData(other.m_TextureData), m_Width(other.m_Width), m_Height(other.m_Height), m_BPP(other.m_BPP), m_Type(other.m_Type)
    {
        other.m_RendererID = 0;
        other.m_TextureData = nullptr;
        other.m_Width = 0;
        other.m_Height = 0;
        other.m_BPP = 0;
    }
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    static void Unbind();

    [[nodiscard]] int GetWidth() const { return m_Width; }
    [[nodiscard]] int GetHeight() const { return m_Height; }
    [[nodiscard]] std::string GetFilePath() const { return m_FilePath; }
    [[nodiscard]] TextureType GetType() const { return m_Type; }
};
