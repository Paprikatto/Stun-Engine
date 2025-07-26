#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int* data, unsigned int count);
    IndexBuffer(const IndexBuffer&) = delete; // Disable copy constructor
    IndexBuffer& operator=(const IndexBuffer&) = delete; // Disable copy assignment operator
    IndexBuffer(IndexBuffer&& other) noexcept : m_RendererID(other.m_RendererID), m_Count(other.m_Count) {
        this->m_RendererID = other.m_RendererID;
        this->m_Count = other.m_Count;
        other.m_RendererID = 0;
        other.m_Count = 0;
    }
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    [[nodiscard]] inline unsigned int GetCount() const { return m_Count; }
};
