#pragma once
#include <bits/shared_ptr.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    VertexArray(const VertexArray&) = delete;
    VertexArray& operator=(const VertexArray&) = delete;
    VertexArray(VertexArray&& other) noexcept : m_RendererID(other.m_RendererID) {
        this->m_RendererID = other.m_RendererID;
        other.m_RendererID = 0;
    }
    ~VertexArray();

    void AddBuffer(const std::shared_ptr<VertexBuffer>& vb, const VertexBufferLayout& layout) const;
    void Bind() const;
    void Unbind() const;
};
