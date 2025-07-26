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
    ~VertexArray();

    void AddBuffer(const std::shared_ptr<VertexBuffer>& vb, const VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;
};
