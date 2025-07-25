#pragma once
#include "Renderer.h"
class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_TextureData;
    int m_Width, m_Height, m_BPP;
public:
    explicit Texture(const std::string& filePath);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind() const;

    inline int GetWidth() const { return m_Width; }
    inline int GetHeight() const { return m_Height; }
};
