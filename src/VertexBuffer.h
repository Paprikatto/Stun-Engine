#pragma once

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    VertexBuffer(const VertexBuffer&) = delete; // Disable copy constructor
    VertexBuffer& operator=(const VertexBuffer&) = delete; // Disable copy assignment operator
    VertexBuffer(VertexBuffer&& other) noexcept : m_RendererID(other.m_RendererID) {
        this->m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    ~VertexBuffer();

    void Bind() const;
    void Unbind() const; 
};
